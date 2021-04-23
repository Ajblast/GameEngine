#include <GRAVEngine.h>
#include <entryPoint.h>

#include "Test2D.h"
#include "Test.h"

class testApp : public GRAVEngine::application
{
public:
	testApp()
	{
		GRAV_LOG_LINE_INFO("%s: Test App Constructor", __FUNCTION__);
		//pushLayer(new Test2D());
		pushLayer(new Test());
	}

	~testApp()
	{
	}
};

GRAVEngine::application* GRAVEngine::createApplication()
{
	return new testApp();
}