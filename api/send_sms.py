from twilio.rest import Client
from dotenv import load_dotenv
import os
load_dotenv()  # take environment variables from .env.

account_sid=os.getenv('account_sid')
auth_token=os.getenv('auth_token')
messaging_service_sid_token=os.getenv('messaging_service_sid_token')

def send_message(message_body,reciver_number):
    client = Client(account_sid, auth_token) 
    
    message = client.messages.create(  
                                messaging_service_sid=messaging_service_sid_token, 
                                body=message_body,      
                                to=reciver_number
                            ) 
    return message.status