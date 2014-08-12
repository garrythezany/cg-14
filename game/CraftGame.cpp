#include "CraftGame.hpp"
#include "ChunkView.hpp"

#include <functional>
#include <chrono>

using namespace lumina;
using namespace std;

CraftGame::CraftGame() {
  m_running = true;
  m_map.addChunk(Vec2i(0,0));
  
  for(int i = 0; i < 16; i++) {
    m_map.setBlockType(Vec3i(i, 0, i), BlockType::Dirt);
    m_map.setBlockType(Vec3i(15-i, 0, i), BlockType::Dirt);
  } 
} 


void CraftGame::init() {
  // configure window
  m_window.setTitle("CG Praktikum 2014 :)");
  m_window.setVersionHint(3, 3);

  // add event callback (capture by reference
  m_window.addEventCallback([&](InputEvent e) {
    // if the inputType was a KeyInput and the key was just pressed and the
    // key was Escape -> set m_running to false to stop program
  return m_camera.processEvent(e, m_window);
    if (e.type == InputType::KeyPressed || e.type == InputType::KeyHold) {

      auto pos = m_camera.getPosition();

      switch (e.keyInput.key) {
        case KeyCode::Escape:
          m_running = false;
          return EventResult::Processed;
          break;
        case KeyCode::Up:
          pos.y += 0.5f;
          m_camera.setPosition(pos);
          break;
        case KeyCode::Down:
          pos.y -= 0.5f;
          m_camera.setPosition(pos);
          break;
        case KeyCode::Left:
          pos.x -= 0.5f;
          m_camera.setPosition(pos);
          break;
        case KeyCode::Right:
          pos.x += 0.5f;
          m_camera.setPosition(pos);
          break;
        case KeyCode::W:
          pos.z += 0.5f;
          m_camera.setPosition(pos);
          break;
        case KeyCode::S:
          pos.z -= 0.5f;
          m_camera.setPosition(pos);
          break;
        default:
          
          break;
      }
    }
    return EventResult::Skipped;
  });
  // resize window
  m_window.resize(Vec2i(1280, 1024));
}

void CraftGame::start() {
  // open the window (we need to call init before!)
  m_window.open();
  // obtain and create render context
  auto& renderContext = m_window.getRenderContext();
  renderContext.create();
  // we just need one context, so we can prime it here just once
  renderContext.prime([this](HotRenderContext& hotContext) {
    this->run(hotContext);
  });
}

void CraftGame::run(lumina::HotRenderContext& hotContext) {
  // load and compile vertex and fragment shader
  VShader vs;
  vs.compile(loadShaderFromFile("shader/CraftGame.vsh"));
  FShader fs;
  fs.compile(loadShaderFromFile("shader/CraftGame.fsh"));

  // create program and link the two shaders
  Program p;
  p.create(vs, fs);

  p.perFragProc.enableDepthTest();
  p.perFragProc.enableDepthWrite();
  p.perFragProc.setDepthFunction(DepthFunction::Less);
  p.primitiveProc.enableCulling();

  auto now = chrono::system_clock::now();

  // run as long as the window is valid and the user hasn't pessed ESC
  while(m_running && m_window.isValid()) {
    auto diff = chrono::system_clock::now() - now;
    float s = chrono::duration_cast<chrono::milliseconds>(diff).count() / 1000.f;
    slog(s, ", FPS:", 1/s);
    now = chrono::system_clock::now();

    // poll events
    m_window.update();
    
    // we need the default FrameBuffer
    hotContext.getDefaultFrameBuffer().prime([&](HotFrameBuffer& hotFB) {
      // clear the background color of the screen
      hotFB.clearColor(0, Color32fA(0, 0, 0, 1));
      hotFB.clearDepth(1.f);

      // prime program to use it
      p.prime([&](HotProgram& hot) {

        hot.uniform["u_view"] = this->m_camera.get_matrix();
        hot.uniform["u_projection"] = this->m_camera.get_ProjectionMatrix();

        Chunk& currentChunk = m_map.getChunk(Vec2i(0,0));
        ChunkView cV(currentChunk,Vec2i(0,0));

        cV.draw(hot);

      });
    });
    // swap buffer
    hotContext.swapBuffer();
  }
}
