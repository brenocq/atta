//--------------------------------------------------
// Atta Processor Module
// interface.h
// Date: 2023-02-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PROCESSOR_BASE_H
#define ATTA_PROCESSOR_BASE_H

namespace atta::processor {

enum class State { IDLE = 0, RUNNING, PAUSED };
enum class Type { CPU_SERIAL = 0, CPU_PARALLEL, GPU, CLUSTER };

}

#endif // ATTA_PROCESSOR_BASE_H
