import requests
from dotenv import load_dotenv
import os
load_dotenv()  # take environment variables from .env.

email_api=str(os.getenv('email_api'))
email_api=str(os.getenv('email'))
api_url=os.getenv('api_url')

def send_email_message(reciver,message_subject,message_body):
	return requests.post(
		f"https://api.mailgun.net/v3/{api_url}/messages",
		auth=("api", email_api),
		data={f"from": "Notification <{email}>",
			"to": [reciver],
			"subject": message_subject,
			"text": message_body})

