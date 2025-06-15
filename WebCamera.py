import cv2
import mediapipe as mp
from deepface import DeepFace
import numpy as np

# Инициализация детекторов
mp_face_detection = mp.solutions.face_detection
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# Ваши персональные данные
YOUR_NAME = "Иван"
YOUR_SURNAME = "Петров"


def detect_emotion(face_image):
    """Определение эмоции с помощью DeepFace"""
    try:
        analysis = DeepFace.analyze(face_image, actions=['emotion'], enforce_detection=False)
        return analysis[0]['dominant_emotion']
    except:
        return "neutral"


def main():
    # Инициализация моделей
    face_detection = mp_face_detection.FaceDetection(min_detection_confidence=0.5)
    hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)

    cap = cv2.VideoCapture(0)

    known_face_encodings = []
    known_face_names = []

    while cap.isOpened():
        success, image = cap.read()
        if not success:
            continue

        # Конвертация цвета и обработка
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        image.flags.writeable = False

        # Детекция лиц
        face_results = face_detection.process(image)

        # Детекция рук
        hand_results = hands.process(image)

        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        fingers_up = 0
        current_face = None

        # Обработка рук
        if hand_results.multi_hand_landmarks:
            for hand_landmarks in hand_results.multi_hand_landmarks:
                # Визуализация руки
                mp_drawing.draw_landmarks(
                    image, hand_landmarks, mp_hands.HAND_CONNECTIONS)

                # Логика подсчёта пальцев
                finger_tips = [4, 8, 12, 16, 20]  # Кончики пальцев
                fingers_up = 0

                # Большой палец
                if hand_landmarks.landmark[4].x < hand_landmarks.landmark[3].x:
                    fingers_up += 1

                # Остальные пальцы
                for tip in finger_tips[1:]:
                    if hand_landmarks.landmark[tip].y < hand_landmarks.landmark[tip - 2].y:
                        fingers_up += 1

        # Обработка лиц
        if face_results.detections:
            for detection in face_results.detections:
                # Получение координат лица
                bboxC = detection.location_data.relative_bounding_box
                ih, iw, _ = image.shape
                x, y, w, h = int(bboxC.xmin * iw), int(bboxC.ymin * ih), \
                    int(bboxC.width * iw), int(bboxC.height * ih)

                # Рисуем прямоугольник вокруг лица
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

                # Вырезаем область лица для анализа
                face_image = image[y:y + h, x:x + w]

                # Определяем, своё ли это лицо (простая проверка)
                # В реальном проекте нужно использовать face recognition
                is_known = False
                if len(known_face_encodings) == 0:
                    # Первое обнаруженное лицо считаем "своим"
                    known_face_encodings.append(face_image)
                    known_face_names.append(YOUR_NAME)
                    is_known = True
                else:
                    # Простое сравнение (в реальном проекте используйте face_recognition.compare_faces)
                    is_known = True  # Упрощение для примера

                # Определение текста для отображения
                if is_known:
                    if fingers_up == 1:
                        text = YOUR_NAME
                    elif fingers_up == 2:
                        text = YOUR_SURNAME
                    elif fingers_up == 3:
                        emotion = detect_emotion(face_image)
                        text = f"Emotion: {emotion}"
                    else:
                        text = "Known face"
                else:
                    text = "Unknown"

                # Отображение текста
                cv2.putText(image, text, (x, y - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

        # Отображение количества пальцев
        cv2.putText(image, f"Fingers: {fingers_up}", (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

        cv2.imshow('Face and Hand Detection', image)

        if cv2.waitKey(5) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
