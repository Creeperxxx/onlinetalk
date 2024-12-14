#include "../eventHandler/eventHandler.h"

int main()
{
    // std::unique_ptr<IEventHandler> eventHandler = std::make_unique<ReactorEventHandler>();
    // eventHandler->run();
    // // std::dynamic_cast<ReactorEventHandler*>(eventHandler.get())->event_loop();
    // // std::dynamic_pointer_cast<ReactorEventHandler>(eventHandler)->event_loop();
    // return 0;

    try
    {
        std::unique_ptr<IEventHandler> eventHandler = std::make_unique<ReactorEventHandler>();
        eventHandler->run();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr <<"异常："<< e.what() << '\n';
    }
}