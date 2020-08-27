// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.
#include "NovelRT.Interop/NovelRTInteropUtils.h"
#include "NovelRT.Interop/Audio/NovelRTAudioService.h"
#include <NovelRT.h>
#include <stdint.h>

#ifdef __cplusplus
using namespace NovelRT;
extern "C" {
#endif

NovelRTAudioService NovelRT_AudioService_create() {
    return reinterpret_cast<NovelRTAudioService>(new Audio::AudioService());
}

NovelRTResult NovelRT_AudioService_destroy(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->~AudioService();
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_initialiseAudio(NovelRTAudioService service, NovelRTBool* output, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    *output = serv->initializeAudio();
    return NOVELRT_SUCCESS;
}


NovelRTResult loadMusic(NovelRTAudioService service, char* input, NovelRTAudioServiceIterator* output, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    auto out = serv->loadMusic(input);
    *output = reinterpret_cast<NovelRTAudioServiceIterator&>(out);
    return NOVELRT_SUCCESS;
}
    
NovelRTResult NovelRT_AudioService_setSoundVolume(NovelRTAudioService service, unsigned int source, float val, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->setSoundVolume(source, val);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_setSoundPosition(NovelRTAudioService service, unsigned int source, float posX, float posY, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->setSoundPosition(source, posX, posY);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_resumeMusic(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->resumeMusic();
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_playMusic(NovelRTAudioService service, NovelRTAudioServiceIterator handle, int loops, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->playMusic(reinterpret_cast<NovelRT::Audio::SoundBank::iterator&>(handle), loops);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_pauseMusic(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->pauseMusic();
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_stopMusic(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->stopMusic();
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_setMusicVolume(NovelRTAudioService service, float value, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->setMusicVolume(value);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_checkSources(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->checkSources();
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_loadSound(NovelRTAudioService service, char* input, unsigned int* output, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    *output = serv->loadSound(input);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_unload(NovelRTAudioService service, unsigned int handle, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->unload(handle);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_playSound(NovelRTAudioService service, unsigned int handle, int loops, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->playSound(handle, loops);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_stopSound(NovelRTAudioService service, unsigned int handle, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->stopSound(handle);
    return NOVELRT_SUCCESS;
}

NovelRTResult NovelRT_AudioService_tearDown(NovelRTAudioService service, const char** errorMessage) {
    if (service == nullptr) {
        if (errorMessage != nullptr) {
            *errorMessage = NovelRT_getErrMsgIsNullptr();
        }
        return NOVELRT_FAILURE;
    }
    auto serv = reinterpret_cast<Audio::AudioService*>(service);
    serv->tearDown();
    return NOVELRT_SUCCESS;
}


#ifdef __cplusplus
}
#endif
