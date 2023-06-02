//--------------------------------------------------
// Atta Processor Module
// processor.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H
#define ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H

#include <atta/processor/base.h>

namespace atta::processor {

class Processor {
  public:
    Processor(Type type);

    Type getType() const;
    State getState() const;

    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;

    virtual void readData(){};
    virtual void writeData(){};

  protected:
    const Type _type;
    State _state;
};

} // namespace atta::processor

#endif // ATTA_PROCESSOR_PROCESSORS_PROCESSOR_H
