#
# -----------------
# pip install SpeechRecognition requests pipwin pyaudio
# -----------------
#
import speech_recognition as sr
import requests
import time

esp32_ip = "ip"  #แก้เป็น IP เรา
recognizer = sr.Recognizer()

def check_voice_control_status():
    try:
        response = requests.get(f"{esp32_ip}/voice_status")
        if response.status_code == 200:
            return response.text.strip() == "true"
        else:
            print("Error getting voice control status.")
            return False
    except requests.RequestException as e:
        print(f"Error checking voice control status: {e}")
        return False

def voice_control():
    with sr.Microphone() as source:
        print("Listening for voice commands...")
        try:
            audio = recognizer.listen(source)
            command = recognizer.recognize_google(audio).lower()
            print(f"Command received: {command}")

            if "open" in command:
                requests.get(f"{esp32_ip}/open")
                print("Curtain opened.")
            elif "close" in command:
                requests.get(f"{esp32_ip}/close")
                print("Curtain closed.")
            else:
                print("Command not recognized.")
        except sr.UnknownValueError:
            print("Sorry, could not understand the audio.")
        except sr.RequestError as e:
            print(f"Could not request results; {e}")

while True:
    if check_voice_control_status(): voice_control()
    else: print("Voice control is disabled. Waiting...")
    time.sleep(2)