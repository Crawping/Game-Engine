#include "RingBufferUnitTest.h"

void Engine::RingBuffer_UnitTest(void)
{
	const unsigned int UTRingBufferSize = 12;

	RingBuffer<unsigned int, UTRingBufferSize>	UTRingBuffer;

	assert(UTRingBuffer.size() == UTRingBufferSize);
	assert(UTRingBuffer.count() == 0);

	for (unsigned int i = 0; i < (UTRingBufferSize * 2.5); i++)
	{
		UTRingBuffer.addItem(i);

		for (unsigned int j = 0; j < UTRingBuffer.count(); j++)
		{
			assert(UTRingBuffer[j] == (i - j));
		}
	}

	UTRingBuffer.clear();
	assert(UTRingBuffer.count() == 0);
}