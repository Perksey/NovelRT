// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#include <NovelRT.h>

using namespace sol;

namespace NovelRT::Lua {
  LuaRunner::LuaRunner(const std::string& fileName) noexcept {
    _fileName = fileName;
  }


  void LuaRunner::init() {
    _state = sol::state();
    _state.open_libraries(lib::base, lib::bit32, lib::coroutine,
      lib::debug, lib::io, lib::math, lib::os,
      lib::package, lib::string, lib::table, lib::utf8);

    auto globalTable = _state.create_table("novelrt");

    _state["novelrt"] = globalTable;

#pragma region NovelRT::
    // Atom
    auto atomType = globalTable.new_usertype<Atom>("Atom", sol::constructors<Atom(), Atom(uintptr_t)>());

    atomType["getNextEventHandlerId"] = &Atom::getNextEventHandlerId;
    atomType["getNextFontSetId"] = &Atom::getNextFontSetId;
    atomType["getNextTextureId"] = &Atom::getNextTextureId;

    // DebugService
    auto debugServiceType = globalTable.new_usertype<DebugService>("DebugService", sol::constructors<DebugService(NovelRunner* const)>());
    debugServiceType["fpsCounterVisible"] = sol::property(
      static_cast<bool (DebugService::*)() const>(&DebugService::getIsFpsCounterVisible),
      static_cast<void (DebugService::*)(bool)>(&DebugService::setIsFpsCounterVisible)
      );

    debugServiceType["framesPerSecond"] = sol::property(
      static_cast<uint32_t(DebugService::*)() const>(&DebugService::getFramesPerSecond),
      static_cast<void (DebugService::*)(uint32_t)>(&DebugService::setFramesPerSecond)
      );


    // LoggingService
    auto loggingServiceType = globalTable.new_usertype<LoggingService>("LoggingService",
      sol::constructors<LoggingService(), LoggingService(const std::string&), LoggingService(const std::string&, LogLevel)>());

    loggingServiceType["log"] = &LoggingService::log;
    loggingServiceType["logInfoLine"] = &LoggingService::logInfoLine;
    loggingServiceType["logErrorLine"] = &LoggingService::logErrorLine;
    loggingServiceType["logWarningLine"] = &LoggingService::logWarningLine;
    loggingServiceType["logDebugLine"] = &LoggingService::logDebugLine;
    loggingServiceType["logInternal"] = &LoggingService::logInternal;
    loggingServiceType["setLogLevel"] = &LoggingService::setLogLevel;

    // TODO: should I include throwIfNullPtr/NotZero?
    // TODO: figure out how to specify template types for logInfo/Error/Warning/Debug

    // LogLevel

    globalTable.new_enum<LogLevel>("LogLevel", {
      { "Debug", LogLevel::Debug },
      { "Info", LogLevel::Info },
      { "Warn", LogLevel::Warn },
      { "Error", LogLevel::Err },
      { "Off", LogLevel::Off }
      });


    // NovelRunner
    auto novelRunnerType = globalTable.new_usertype<NovelRunner>("NovelRunner",
      sol::constructors<NovelRunner(int), NovelRunner(int, const std::string&), NovelRunner(int, const std::string&, uint32_t)>()
      );

    novelRunnerType["runNovel"] = &NovelRunner::runNovel;
    novelRunnerType["renderer"] = sol::property(&NovelRunner::getRenderer);
    novelRunnerType["interactionService"] = sol::property(&NovelRunner::getInteractionService);
    novelRunnerType["audioService"] = sol::property(&NovelRunner::getAudioService);
    novelRunnerType["debugService"] = sol::property(&NovelRunner::getDebugService);
    novelRunnerType["dotNetRuntimeService"] = sol::property(&NovelRunner::getDotNetRuntimeService);
    novelRunnerType["windowingService"] = sol::property(&NovelRunner::getWindowingService);

    // Transform
    auto transformType = globalTable.new_usertype<Transform>("Transform",
      sol::constructors<Transform(),
      Transform(const Maths::GeoVector2<float> & position,
        float rotation, const Maths::GeoVector2<float> & scale)>()
      );

    transformType["AABB"] = sol::property(&Transform::getAABB);
    transformType["bounds"] = sol::property(&Transform::getBounds);
    transformType["position"] = sol::property(
      static_cast<const Maths::GeoVector2<float> & (Transform::*)() const>(&Transform::position),
      static_cast<Maths::GeoVector2<float>& (Transform::*)()>(&Transform::position)
      );

    transformType["rotation"] = sol::property(
      static_cast<const float& (Transform::*)() const>(&Transform::rotation),
      static_cast<float& (Transform::*)()>(&Transform::rotation)
      );

    transformType["scale"] = sol::property(
      static_cast<const Maths::GeoVector2<float> & (Transform::*)() const>(&Transform::scale),
      static_cast<Maths::GeoVector2<float>& (Transform::*)()>(&Transform::scale)
      );

    // WorldObject is left out, because it is an abstract class and isn't consumed or returned in any parts of the api.

#pragma endregion

#pragma region NovelRT::Animation

    //AnimatorPlayState

    globalTable.new_enum<Animation::AnimatorPlayState>("AnimatorPlayState", {
      { "Paused", Animation::AnimatorPlayState::Paused },
      { "Playing", Animation::AnimatorPlayState::Playing },
      { "Stopped", Animation::AnimatorPlayState::Stopped }
      });

    //SpriteAnimator

    auto spriteAnimatorType = globalTable.new_usertype<Animation::SpriteAnimator>("SpriteAnimator",
      sol::constructors<Animation::SpriteAnimator(NovelRunner*, Graphics::ImageRect*)>());

    spriteAnimatorType["play"] = &Animation::SpriteAnimator::play;
    spriteAnimatorType["pause"] = &Animation::SpriteAnimator::pause;
    spriteAnimatorType["stop"] = &Animation::SpriteAnimator::stop;
    spriteAnimatorType["currentPlayState"] = sol::property(&Animation::SpriteAnimator::getCurrentPlayState);
    spriteAnimatorType["insertNewState"] = &Animation::SpriteAnimator::insertNewState;

    //SpriteAnimatorFrame

    auto spriteAnimatorFrameType = globalTable.new_usertype<Animation::SpriteAnimatorFrame>("SpriteAnimatorFrame",
      sol::constructors<Animation::SpriteAnimatorFrame()>());

    spriteAnimatorFrameType["frameEnter"] = &Animation::SpriteAnimatorFrame::FrameEnter;
    spriteAnimatorFrameType["frameExit"] = &Animation::SpriteAnimatorFrame::FrameExit;

    spriteAnimatorFrameType["texture"] = sol::property(
      static_cast<const std::shared_ptr<Graphics::Texture> & (Animation::SpriteAnimatorFrame::*)() const>(&Animation::SpriteAnimatorFrame::texture),
      static_cast<std::shared_ptr<Graphics::Texture> & (Animation::SpriteAnimatorFrame::*)()>(&Animation::SpriteAnimatorFrame::texture)
      );

    spriteAnimatorFrameType["duration"] = sol::property(
      static_cast<const Timing::Timestamp & (Animation::SpriteAnimatorFrame::*)() const>(&Animation::SpriteAnimatorFrame::duration),
      static_cast<Timing::Timestamp & (Animation::SpriteAnimatorFrame::*)()>(&Animation::SpriteAnimatorFrame::duration)
      );

    //SpriteAnimatorState

    auto spriteAnimatorStateType = globalTable.new_usertype<Animation::SpriteAnimatorState>("SpriteAnimatorState");

    spriteAnimatorStateType["insertNewState"] = &Animation::SpriteAnimatorState::insertNewState;
    spriteAnimatorStateType["removeStateAtIndex"] = &Animation::SpriteAnimatorState::removeStateAtIndex;
    spriteAnimatorStateType["shouldLoop"] = sol::property(
      static_cast<const bool& (Animation::SpriteAnimatorState::*)() const>(&Animation::SpriteAnimatorState::shouldLoop),
      static_cast<bool& (Animation::SpriteAnimatorState::*)()>(&Animation::SpriteAnimatorState::shouldLoop)
      );
    spriteAnimatorStateType["frames"] = sol::property(
      static_cast<const std::vector<Animation::SpriteAnimatorFrame> & (Animation::SpriteAnimatorState::*)() const>(&Animation::SpriteAnimatorState::frames),
      static_cast<std::vector<Animation::SpriteAnimatorFrame> & (Animation::SpriteAnimatorState::*)()>(&Animation::SpriteAnimatorState::frames)
      );
    spriteAnimatorStateType["tryFindValidTransition"] = &Animation::SpriteAnimatorState::tryFindValidTransition;

#pragma endregion

#pragma region NovelRT::Audio
    auto audioServiceType = globalTable.new_usertype<Audio::AudioService>("AudioService", sol::constructors<Audio::AudioService()>());
    audioServiceType["isInitialized"] = &Audio::AudioService::isInitialised;
    audioServiceType["initializeAudio"] = &Audio::AudioService::initializeAudio;
    audioServiceType["loadMusic"] = &Audio::AudioService::loadMusic;
    audioServiceType["setSoundVolume"] = &Audio::AudioService::setSoundVolume;
    audioServiceType["setSoundPosition"] = &Audio::AudioService::setSoundPosition;
    audioServiceType["resumeMusic"] = &Audio::AudioService::resumeMusic;
    audioServiceType["playMusic"] = &Audio::AudioService::playMusic;
    audioServiceType["pauseMusic"] = &Audio::AudioService::pauseMusic;
    audioServiceType["stopMusic"] = &Audio::AudioService::stopMusic;
    audioServiceType["setMusicVolume"] = &Audio::AudioService::setMusicVolume;
    audioServiceType["checkSources"] = &Audio::AudioService::checkSources;
    audioServiceType["loadSound"] = &Audio::AudioService::loadSound;
    audioServiceType["unload"] = &Audio::AudioService::unload;
    audioServiceType["playSound"] = &Audio::AudioService::playSound;
    audioServiceType["stopSound"] = &Audio::AudioService::stopSound;
       
#pragma endregion

#pragma region NovelRT::DotNet
#pragma endregion

#pragma region NovelRT::Graphics

    //BasicFillRect

    auto basicFillRectType = globalTable.new_usertype<Graphics::BasicFillRect>("BasicFillRect",
      sol::constructors<Graphics::BasicFillRect(const Transform&, int, std::weak_ptr<Graphics::Camera>,
        Graphics::ShaderProgram, const Graphics::RGBAConfig&)>());

    basicFillRectType["drawObject"] = &Graphics::BasicFillRect::drawObject;
    basicFillRectType["colourConfig"] = sol::property(
      static_cast<const Graphics::RGBAConfig (Graphics::BasicFillRect::*)() const>(&Graphics::BasicFillRect::getColourConfig),
      static_cast<void (Graphics::BasicFillRect::*)(const Graphics::RGBAConfig&)>(&Graphics::BasicFillRect::setColourConfig)
      );

    //Camera

    auto cameraType = globalTable.new_usertype<Graphics::Camera>("Camera", sol::constructors<Graphics::Camera()>());

    cameraType["viewMatrix"] = sol::property(&Graphics::Camera::getViewMatrix, &Graphics::Camera::setViewMatrix);

    //TODO: Graphics::Camera::getViewMatrix/getProjectMatrix appears to have getters/setters, but also set methods. Bug Matt.

    cameraType["cameraUboMatrix"] = sol::property(&Graphics::Camera::getCameraUboMatrix);
    cameraType["frameState"] = sol::property(&Graphics::Camera::getFrameState);
    cameraType["createDefaultOrthographicProjection"] = &Graphics::Camera::createDefaultOrthographicProjection;

    //CameraFrameState

    globalTable.new_enum<Graphics::CameraFrameState>("CameraFrameState", {
      { "Unmodified", Graphics::CameraFrameState::Unmodified },
      { "ModifiedInCurrent", Graphics::CameraFrameState::ModifiedInCurrent },
      { "ModifiedInLast", Graphics::CameraFrameState::ModifiedInLast }
    });

    //FontSetType

    auto fontSetType = globalTable.new_usertype<Graphics::FontSet>("FontSet", sol::constructors<Graphics::FontSet(std::weak_ptr<Graphics::RenderingService>, Atom)>());
    fontSetType["loadFontAsTextureSet"] = &Graphics::FontSet::loadFontAsTextureSet;
    fontSetType["fontFile"] = sol::property(&Graphics::FontSet::getFontFile);
    fontSetType["fontSize"] = sol::property(&Graphics::FontSet::getFontSize);

    // GraphicsCharacterRenderData

    auto graphicsCharacterRenderDataType = globalTable.new_usertype<Graphics::GraphicsCharacterRenderData>("GraphicsCharacterRenderData");
    graphicsCharacterRenderDataType["texture"] = &Graphics::GraphicsCharacterRenderData::texture;
    graphicsCharacterRenderDataType["size"] = &Graphics::GraphicsCharacterRenderData::size;
    graphicsCharacterRenderDataType["bearing"] = &Graphics::GraphicsCharacterRenderData::bearing;
    graphicsCharacterRenderDataType["advance"] = &Graphics::GraphicsCharacterRenderData::advance;

    //ImageRect
    auto imageRectType = globalTable.new_usertype<Graphics::ImageRect>("ImageRect",
      sol::constructors<Graphics::ImageRect(const Transform&, int, Graphics::ShaderProgram, std::weak_ptr<Graphics::Camera>, const Graphics::RGBAConfig&),
      Graphics::ImageRect(const Transform&, int, Graphics::ShaderProgram, std::weak_ptr<Graphics::Camera>, std::shared_ptr<Graphics::Texture>, const Graphics::RGBAConfig&)>()
      );

    imageRectType["texture"] = sol::property(
      static_cast<const std::shared_ptr<Graphics::Texture>& (Graphics::ImageRect::*)() const>(&Graphics::ImageRect::texture),
      static_cast<std::shared_ptr<Graphics::Texture> & (Graphics::ImageRect::*)()>(&Graphics::ImageRect::texture)
      );

    imageRectType["colourTint"] = sol::property(
      static_cast<const Graphics::RGBAConfig& (Graphics::ImageRect::*)() const>(&Graphics::ImageRect::colourTint),
      static_cast<Graphics::RGBAConfig & (Graphics::ImageRect::*)()>(&Graphics::ImageRect::colourTint)
      );


    imageRectType["drawObject"] = &Graphics::ImageRect::drawObject;
    imageRectType["colourTint"] = sol::property(
      static_cast<const Graphics::RGBAConfig & (Graphics::ImageRect::*)() const>(&Graphics::ImageRect::colourTint),
      static_cast<Graphics::RGBAConfig & (Graphics::ImageRect::*)()>(&Graphics::ImageRect::colourTint));


    //RenderService

    auto renderingServiceType = globalTable.new_usertype<Graphics::RenderingService>("RenderService",
      sol::constructors<Graphics::RenderingService(NovelRunner* const)>());

    renderingServiceType["createImageRect"] = sol::overload(
      static_cast<std::unique_ptr<Graphics::ImageRect>(Graphics::RenderingService::*)(const Transform&, int, const std::string&, const Graphics::RGBAConfig&)>(&Graphics::RenderingService::createImageRect),
      static_cast<std::unique_ptr<Graphics::ImageRect>(Graphics::RenderingService::*)(const Transform&, int, const Graphics::RGBAConfig&)>(&Graphics::RenderingService::createImageRect)
    );

    renderingServiceType["createBasicFillRect"] = &Graphics::RenderingService::createBasicFillRect;
    renderingServiceType["createTextRect"] = &Graphics::RenderingService::createTextRect;
    renderingServiceType["camera"] = sol::property(&Graphics::RenderingService::getCamera);
    renderingServiceType["beginFrame"] = &Graphics::RenderingService::beginFrame;
    renderingServiceType["endFrame"] = &Graphics::RenderingService::endFrame;
    renderingServiceType["texture"] = sol::property(
      reinterpret_cast<std::shared_ptr<Graphics::Texture> (Graphics::RenderingService::*)(void)>(&Graphics::RenderingService::getTexture)
      );

    renderingServiceType["getTexture"] = static_cast<std::shared_ptr<Graphics::Texture>(Graphics::RenderingService::*)(const std::string&)>(&Graphics::RenderingService::getTexture);
    renderingServiceType["fontSet"] = sol::property(&Graphics::RenderingService::getFontSet);

    //RenderObject

    //TODO: RenderObject is abstract; how do you represent this in lua?

    //auto renderObjectType = globalTable.new_usertype<Graphics::RenderObject>("RenderObject",
    //  sol::constructors<Graphics::RenderObject(const Transform&, int, Graphics::ShaderProgram, std::weak_ptr<Graphics::Camera>)>());

    //renderObjectType["executeObjectBehaviour"] = &Graphics::RenderObject::executeObjectBehaviour;

    //RGBAConfig

    auto rgbaConfigType = globalTable.new_usertype<Graphics::RGBAConfig>("RGBAConfig", sol::constructors<Graphics::RGBAConfig(int, int, int, int)>());

    rgbaConfigType["r"] = sol::property(
      static_cast<int (Graphics::RGBAConfig::*)() const>(&Graphics::RGBAConfig::getR),
      reinterpret_cast<void (Graphics::RGBAConfig::*)(int)>(&Graphics::RGBAConfig::getR)
      );

    rgbaConfigType["getRScalar"] = &Graphics::RGBAConfig::getRScalar;

    rgbaConfigType["g"] = sol::property(
      static_cast<int (Graphics::RGBAConfig::*)() const>(&Graphics::RGBAConfig::getG),
      reinterpret_cast<void (Graphics::RGBAConfig::*)(int)>(&Graphics::RGBAConfig::getG)
      );

    rgbaConfigType["getGScalar"] = &Graphics::RGBAConfig::getGScalar;

    rgbaConfigType["b"] = sol::property(
      static_cast<int (Graphics::RGBAConfig::*)() const>(&Graphics::RGBAConfig::getB),
      reinterpret_cast<void (Graphics::RGBAConfig::*)(int)>(&Graphics::RGBAConfig::getB)
      );

    rgbaConfigType["getBScalar"] = &Graphics::RGBAConfig::getBScalar;

    rgbaConfigType["a"] = sol::property(
      static_cast<int (Graphics::RGBAConfig::*)() const>(&Graphics::RGBAConfig::getA),
      reinterpret_cast<void (Graphics::RGBAConfig::*)(int)>(&Graphics::RGBAConfig::getA)
      );

    rgbaConfigType["getAScalar"] = &Graphics::RGBAConfig::getAScalar;

    //ShaderProgram

    auto shaderProgramType = globalTable.new_usertype<Graphics::ShaderProgram>(sol::constructors<Graphics::ShaderProgram()>());
    shaderProgramType["shaderProgramId"] = &Graphics::ShaderProgram::shaderProgramId;
    shaderProgramType["finalViewMatrixBufferUboId"] = &Graphics::ShaderProgram::finalViewMatrixBufferUboId;
    shaderProgramType["uboIds"] = &Graphics::ShaderProgram::uboIds;

    //TextRect

    auto textRectType = globalTable.new_usertype<Graphics::TextRect>("TextRect",
      sol::constructors<Graphics::TextRect(const Transform&,
        int layer,
        Graphics::ShaderProgram,
        std::weak_ptr<Graphics::Camera>,
        std::shared_ptr<Graphics::FontSet>,
        const Graphics::RGBAConfig&)>()
      );

    textRectType["colourConfig"] = sol::property(&Graphics::TextRect::getColourConfig, &Graphics::TextRect::setColourConfig);

    //TODO: again, seems to have a setter method, but also get/set method properties. Bug Matt about this also.

    textRectType["text"] = sol::property(
      static_cast<std::string(Graphics::TextRect::*)() const>(&Graphics::TextRect::getText),
      reinterpret_cast<void (Graphics::TextRect::*)(const std::string&)>(&Graphics::TextRect::getText)
      );

    textRectType["setActive"] = &Graphics::TextRect::setActive;

    textRectType["fontSet"] = sol::property(
      static_cast<std::shared_ptr<Graphics::FontSet>(Graphics::TextRect::*)() const>(&Graphics::TextRect::getFontSet),
      static_cast<void (Graphics::TextRect::*)(std::shared_ptr<Graphics::FontSet>)>(&Graphics::TextRect::setFontSet)
      );

    auto textureType = globalTable.new_usertype<Graphics::Texture>("Texture",
      sol::constructors<Graphics::Texture(std::weak_ptr<Graphics::RenderingService>, Atom)>());

    textureType["loadPngAsTexture"] = &Graphics::Texture::loadPngAsTexture;
    textureType["textureFile"] = sol::property(&Graphics::Texture::getTextureFile);
    textureType["size"] = sol::property(&Graphics::Texture::getSize);

#pragma endregion


  }


  void LuaRunner::run() {
    try {
      auto result = _state.safe_script_file(_fileName);
    }
    catch (const sol::error& err) {
      std::cout << "An error occurred from within sol: " << err.what() << std::endl;
    }
    
  }
}
