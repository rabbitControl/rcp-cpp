#include <catch2/reporters/catch_reporter_event_listener.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>

#include <rcp/MemoryTools.h>


#ifdef DO_MEMORY_TEST

class endRunListener
    : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunEnded(Catch::TestRunStats const& stats) override
    {
        rcp::MemoryTools::report();
    }
};

CATCH_REGISTER_LISTENER(endRunListener)

#endif
