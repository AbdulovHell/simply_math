#include "math_obj.h"

using namespace Project::Core;

Project::Core::math_obj::math_obj()
{
	//point_left = nullptr;
	//point_right = nullptr;
	//point_collar = nullptr;
}

Project::Core::math_obj::math_obj(leaf_ptr _pl, leaf_ptr _pr, leaf_ptr _pc)
{
	point_left = _pl;
	point_right = _pr;
	point_collar = _pc;
}

Project::Core::math_obj::~math_obj()
{
	//уже мемвиз подтирает, или еще не?
}

flags Project::Core::math_obj::get_class_type()
{
	return flags::base;
}

size_t Project::Core::math_obj::get_sizeof()
{
	return sizeof(*this);
}

void * Project::Core::math_obj::get_this()
{
	return this;
}

leaf_ptr Project::Core::math_obj::get_pl()
{
	return point_left;
}

void Project::Core::math_obj::assing_pl(leaf_ptr _pointer)
{
	point_left = _pointer;
}

leaf_ptr Project::Core::math_obj::get_pr()
{
	return point_right;
}

void Project::Core::math_obj::assing_pr(leaf_ptr _pointer)
{
	point_right = _pointer;
}

leaf_ptr Project::Core::math_obj::get_pc()
{
	return point_collar;
}

void Project::Core::math_obj::assing_pc(leaf_ptr _pointer)
{
	point_collar = _pointer;
}

void Project::Core::math_obj::copy_to(void * _ptr)
{
	//math_obj temp;
	//std::memcpy(_ptr, &temp, temp.get_sizeof());
	//math_obj* place = (math_obj*)_ptr;
	//place->copy(this);
}

bool Project::Core::math_obj::copy(math_obj * _original)
{
	point_left = _original->point_left;
	point_right = _original->point_right;
	point_collar = _original->point_collar;
	return true;
}
