#include <GRAVEngine.h>
#include <entryPoint.h>

class testApp : public GRAVEngine::application
{
public:
	testApp()
	{
		GRAV_LOG_LINE_INFO("%s: Test App Constructor", __FUNCTION__);
	}

	~testApp()
	{
	}
};

GRAVEngine::application* GRAVEngine::createApplication()
{
	return new testApp();
}