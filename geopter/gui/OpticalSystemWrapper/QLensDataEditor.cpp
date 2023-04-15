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
    return parent_->GetOpticalAssembly()->GetSurface(i)->Radius();
}

double QLensDataEditor::GetThicknessAt(int i) const
{
    return parent_->GetOpticalAssembly()->GetGap(i)->Thickness();
}

QString QLensDataEditor::GetMaterialAt(int i) const
{
    return QString().fromStdString( parent_->GetOpticalAssembly()->GetGap(i)->GetMaterial()->Name() );
}

QString QLensDataEditor::GetLabelAt(int i) const
{
    return QString().fromStdString( parent_->GetOpticalAssembly()->GetSurface(i)->Label() );
}

void QLensDataEditor::InsertNewSurfaceAt(int i)
{
    parent_->GetOpticalAssembly()->Insert(i);
}

void QLensDataEditor::SetSurfaceDataAt(int i, double radius, double thick, QString material, QString comment)
{
    parent_->GetOpticalAssembly()->GetSurface(i)->SetRadius(radius);
    parent_->GetOpticalAssembly()->GetGap(i)->SetThickness(thick);
    parent_->GetOpticalAssembly()->GetGap(i)->SetMaterial( MaterialLibrary::Find(material.toStdString()) );
    parent_->GetOpticalAssembly()->GetSurface(i)->SetLabel(comment.toStdString());
}

void QLensDataEditor::SetRadiusAt(int i, double r)
{
    parent_->GetOpticalAssembly()->GetSurface(i)->SetRadius(r);
}

void QLensDataEditor::SetThicknessAt(int i, double t)
{
    parent_->GetOpticalAssembly()->GetGap(i)->SetThickness(t);
}

void QLensDataEditor::SetMaterialAt(int i, QString material)
{
    parent_->GetOpticalAssembly()->GetGap(i)->SetMaterial(MaterialLibrary::Find(material.toStdString()));
}

