#ifndef VIDEODISPLAYVIEWPOOL_H
#define VIDEODISPLAYVIEWPOOL_H

#include "videodisplayunit.h"
#include <list>
#include <mutex>

class VideoDisplayViewPool
{
public:
    explicit VideoDisplayViewPool(int poolSize);
    ~VideoDisplayViewPool();

    VideoDisplayUnit* acquire();
    void release(VideoDisplayUnit* widget);

private:
    std::list<VideoDisplayUnit*> m_pool;
    std::mutex m_mutex;
    int m_poolSize;
};

#endif // VIDEODISPLAYVIEWPOOL_H
