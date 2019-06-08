#include <ctime>
#include "OSystem.hxx"

OSystem::OSystem()
{
    myNVRamDir = ".";
    mySettings = 0;
    myFrameBuffer = new FrameBuffer();
    mySound = new SoundSDL(this);
    mySerialPort = new SerialPort();
    myEventHandler = new EventHandler(this);
    myPropSet = new PropertiesSet(this);
    Paddles::setDigitalSensitivity(5);
    Paddles::setMouseSensitivity(5);
}

OSystem::~OSystem()
{
    delete myFrameBuffer;
    delete mySound;
    delete mySerialPort;
    delete myEventHandler;
    delete myPropSet;
}

bool OSystem::create() { return 1; }

void OSystem::mainLoop() { }

void OSystem::pollEvent() { }

bool OSystem::queryVideoHardware() { return 1; }

void OSystem::stateChanged(EventHandler::State state) { }

void OSystem::setDefaultJoymap(Event::Type event, EventMode mode) { }

void OSystem::setFramerate(float framerate) { }
//void OSystem::setFramerate(float framerate)
//{
//    if(framerate > 0.0)
//    {
//        myDisplayFrameRate = framerate;
//        myTimePerFrame = (uInt32)(1000000.0 / myDisplayFrameRate);
//    }
//}

uInt64 OSystem::getTicks() const
{
    return myConsole->tia().getMilliSeconds();
}

EventHandler::EventHandler(OSystem*)
{
    
}

EventHandler::~EventHandler()
{
    
}

FrameBuffer::FrameBuffer()
{

}

FrameBuffer::~FrameBuffer()
{

}

FBInitStatus FrameBuffer::initialize(const string& title, uInt32 width, uInt32 height)
{
    //logMsg("called FrameBuffer::initialize, %d,%d", width, height);
    return kSuccess;
}

void FrameBuffer::refresh() { }

void FrameBuffer::showFrameStats(bool enable) { }

// 0 to <counts> - 1, i_s caches the value of counts
//#define iterateTimes(counts, i) for(unsigned int i = 0, i ## _s = counts; i < (i ## _s); i++)
//void FrameBuffer::setTIAPalette(const uInt32* palette)
//{
//}
