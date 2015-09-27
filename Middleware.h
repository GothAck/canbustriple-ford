
#ifndef CANMiddleware_H
#define CANMiddleware_H

class Middleware
{
  public:
    virtual void tick();
    virtual bool process(Message* msg);
    virtual void commandHandler(byte* bytes, int length);
    // boolean enabled;
    Middleware(){};
    ~Middleware(){};
};

bool Middleware::process( Message* msg ){ return true; }


#endif
