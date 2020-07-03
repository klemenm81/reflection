#pragma once

class IRESTController {
public:
    virtual void start() = 0;
    virtual void shutdown() = 0;
    virtual ~IRESTController() {
    }
};
