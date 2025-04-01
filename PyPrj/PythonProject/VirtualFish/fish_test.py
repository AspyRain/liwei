import cv2

def main(video_source=0):
    # 打开摄像头
    cap = cv2.VideoCapture(video_source)

    # 检查摄像头是否成功打开
    if not cap.isOpened():
        print("无法打开摄像头")
        return

    while True:
        # 捕获每一帧
        ret, frame = cap.read()

        if not ret:
            print("无法读取帧")
            break

        # 显示捕获的帧
        cv2.imshow("摄像头", frame)

        # 如果按下 'q' 键，退出循环
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # 释放摄像头并关闭所有窗口
    cap.release()
    cv2.destroyAllWindows()

# 调用函数
main(video_source=0)
