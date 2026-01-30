#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// Gọi biến Queue từ bên C sang
#include "cmsis_os.h"
extern "C" {
    extern osMessageQueueId_t moveQueueHandle;
}

Model::Model() : modelListener(0), currentDirection(0)
{
}

// Hàm này chạy mỗi khung hình (60Hz)
void Model::tick()
{
    // Kiểm tra xem có tin nhắn nào trong Queue không
    if (moveQueueHandle != NULL)
    {
        int receivedDir = 0;
        // Lấy tin nhắn ra, timeout = 0 (không chờ)
        if (osMessageQueueGet(moveQueueHandle, &receivedDir, NULL, 0) == osOK)
        {
            currentDirection = receivedDir;
        }
    }
}

// Hàm để View gọi lấy giá trị
int Model::getMoveDirection()
{
    return currentDirection;
}
