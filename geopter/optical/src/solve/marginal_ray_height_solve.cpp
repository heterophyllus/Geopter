/**
This file is part of Geopter.
Geopter is free software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

Geopter is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Geopter.
If not, see <https://www.gnu.org/licenses/>.
**/

#include "solve/marginal_ray_height_solve.h"
#include "system/optical_system.h"
#include "paraxial/paraxial_trace.h"
#include "sequential/sequential_trace.h"

using namespace geopter;


MarginalHeightSolve::MarginalHeightSolve(int gi, double value, double zone) :
    Solve()
{
    solve_type_ = SolveType::MarginalHeight;
    gap_index_ = gi;
    height_ = value;
    pupil_zone_ = zone;
}

bool MarginalHeightSolve::Check(const OpticalSystem * /*opt_sys*/)
{
    return true;
}

void MarginalHeightSolve::Apply(OpticalSystem* opt_sys)
{
    const double ref_wvl = opt_sys->GetOpticalSpec()->GetWavelengthSpec()->ReferenceWavelength();
    const int surface_index = gap_index_;

    if(fabs(pupil_zone_) < std::numeric_limits<double>::epsilon()){ // paraxial

        ParaxialTrace* tracer = new ParaxialTrace(opt_sys);

        double y0 = opt_sys->GetFirstOrderData()->reference_y0;
        double u0 = opt_sys->GetFirstOrderData()->reference_u0;

        assert(fabs(y0) <= std::numeric_limits<double>::epsilon());
        //u0 *= pupil_zone_;

        auto ax_ray = tracer->TraceParaxialRayFromObject(y0, u0, ref_wvl);

        double u_prime = ax_ray->At(surface_index).u_prime;
        double y       = ax_ray->At(surface_index).y;

        double t = (height_ - y)/u_prime;

        delete tracer;
        // set value
        opt_sys->GetOpticalAssembly()->GetGap(gap_index_)->SetThickness(t);

    }else{
        SequentialTrace *tracer = new SequentialTrace(opt_sys);
        SequentialPath seq_path = tracer->CreateSequentialPath(ref_wvl);
        Eigen::Vector2d pupil({0.0, pupil_zone_});
        Field* fld = opt_sys->GetOpticalSpec()->GetFieldSpec()->GetField(0);
        auto ray = std::make_shared<Ray>(seq_path.Size());
        tracer->TracePupilRay(ray, seq_path, pupil, fld, ref_wvl);

        double y = ray->GetSegmentAt(surface_index)->Y();
        double z = ray->GetSegmentAt(surface_index)->Z(); // surface sag
        double M = ray->GetSegmentAt(surface_index)->M();
        double N = ray->GetSegmentAt(surface_index)->N();
        double tanU = M/N;

        double t = (height_ - y)/tanU + z;

        delete tracer;

        opt_sys->GetOpticalAssembly()->GetGap(gap_index_)->SetThickness(t);
    }

}

void MarginalHeightSolve::SetParameters(int index, double param1, double param2, double param3)
{

}
