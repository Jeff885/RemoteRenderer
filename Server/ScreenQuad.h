#ifndef SCREEN_QUAD
#define SCREEN_QUAD
#include <Windows.h>
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/ref_ptr>
#include <osg/Switch>
#include <osgDB/ReadFile>

class ScreenQuad: public osg::Geode
{

public:
	ScreenQuad()
	{
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
		vertices->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
		vertices->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
		vertices->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
		vertices->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));

		osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
		texcoords->push_back(osg::Vec2(0.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 0.0f));
		texcoords->push_back(osg::Vec2(1.0f, 1.0f));
		texcoords->push_back(osg::Vec2(0.0f, 1.0f));

		osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
		quad->setVertexArray(vertices.get());
		quad->setTexCoordArray(0,texcoords.get());
		quad->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4) );
		this->addDrawable(quad.get());
	}
};

#endif