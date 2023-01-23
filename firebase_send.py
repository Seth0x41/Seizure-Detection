
import firebase_admin
from firebase_admin import credentials, messaging
firebase_cred = credentials.Certificate("firebasesecret.json")
firebase_app = firebase_admin.initialize_app(firebase_cred)

def send_topic_push(body,title="Alert!"):
    topic = "BPM"
    message = messaging.Message(
        notification=messaging.Notification(
            title=title,
            body=body
            ),
            
            topic=topic
            )
    messaging.send(message)


