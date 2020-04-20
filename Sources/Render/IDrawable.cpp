#include <Render/IDrawable.h>

//Material * DrawableList::GetMaterial(IDrawable * drawable) const
//{
//	Material* mat = (*(*this).find(drawable)).second.material;
//	return mat;
//}
//
//void DrawableList::SetMaterial(IDrawable * drawable, Material * mtl)
//{
//	(*this)[drawable].material = mtl;
//}
//
//void DrawableList::SetModelMat(IDrawable * drawable, glm::mat4 & mat)
//{
//	(*this)[drawable].modelMat = mat;
//}


void DrawableList::AddContext(DrawableContext & context)
{
	this->insert(std::make_pair(context.GetID(), context));
}
