#pragma once

#include "lumina/lumina.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include "MapView.hpp"
#include "ChunkGenerator.hpp"

class CraftGame {
public:
  CraftGame();

  void init();
  void start();

private:
  lumina::Window m_window;
  ChunkGenerator m_chunkGenerator;
  bool m_running;
  Camera m_camera;
  Map m_map;
  MapView m_mapView;

  void run(lumina::HotRenderContext& hotContext);

  lumina::EventResult onEvent(lumina::InputEvent e);
};
