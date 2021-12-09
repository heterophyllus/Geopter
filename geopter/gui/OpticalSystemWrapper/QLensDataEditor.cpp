#include "QLensDataEditor.h"
#include "QOpticalSystem.h"

QLensDataEditor::QLensDataEditor(QOpticalSystem* opt_sys)
{
    parent_ = opt_sys;
}

QLensDataEditor::~QLensDataEditor()
{
    parent_ = nullptr;
}

double QLensDataEditor::GetRadiusAt(int i) const
{
    return parent_->optical_assembly()->surface(i)->profile()->radius();
}

double QLensDataEditor::GetThicknessAt(int i) const
{
    return parent_->optical_assembly()->gap(i)->thi();
}

QString QLensDataEditor::GetMaterialAt(int i) const
{
    return QString().fromStdString( parent_->optical_assembly()->gap(i)->material()->name() );
}

QString QLensDataEditor::GetLabelAt(int i) const
{
    return QString().fromStdString( parent_->optical_assembly()->surface(i)->label() );
}

void QLensDataEditor::InsertNewSurfaceAt(int i)
{
    parent_->optical_assembly()->insert(i);
}

void QLensDataEditor::SetSurfaceDataAt(int i, double radius, double thick, QString material, QString comment)
{
    parent_->optical_assembly()->surface(i)->profile()->set_radius(radius);
    parent_->optical_assembly()->gap(i)->set_thi(thick);
    parent_->optical_assembly()->gap(i)->set_material( MaterialLibrary::find(material.toStdString()) );
    parent_->optical_assembly()->surface(i)->set_label(comment.toStdString());
}

void QLensDataEditor::SetRadiusAt(int i, double r)
{
    parent_->optical_assembly()->surface(i)->profile()->set_radius(r);
}

void QLensDataEditor::SetThicknessAt(int i, double t)
{
    parent_->optical_assembly()->gap(i)->set_thi(t);
}

void QLensDataEditor::SetMaterialAt(int i, QString material)
{
    parent_->optical_assembly()->gap(i)->set_material(MaterialLibrary::find(material.toStdString()));
}

