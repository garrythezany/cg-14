#pragma once

#include <map>

#include "ChunkView.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Environment.hpp"
#include "TexArray.hpp"

class MapView
{

public:

	MapView(Map& map, Camera& cam, Environment& envir);
	void drawNormalPass(Mat4f viewMat, Mat4f projMat);
	void drawFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBuffer, Tex2D& dBuffer);
	void init();
	void notifyBlockUpdate(Vec3i blockPos);
	void deleteChunkView(Vec2i chunkPos);
	void resetMapView();

	/**
	* Checks if a chunk is visible
	*/
	bool isChunkVisible(Vec2i& chunkPos);
	bool exists(Vec2i pos);
	void clearMapView(Vec2i position);
	int size(){return m_mapView.size();}
	

private:

	void drawChunks(HotProgram& hotP, HotTexCont& hotTexCont);

	map<Vec2i, ChunkView> m_mapView;
	Map& m_map;
	Camera& m_cam;
	Environment& m_envir;
	TexArray m_colorTexture;
	Program m_program;
	Program m_normalPass;
	Program m_finalPass;
	TexArray m_normalTexture;
	Tex2D m_pickaxeTexture;
};