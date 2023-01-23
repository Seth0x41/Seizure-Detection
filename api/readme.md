# introduction
This code is a Flask application that provides several endpoints for sending and retrieving data, as well as sending notifications via SMS, email, and Firebase push notifications. The application is built to monitor heart rate and detect seizures, heart problems and fall detection. When the application detects any of these situations, it will send notifications via SMS, email and Firebase push notifications to the specified recipients. The application also provides a way to retrieve the heart rate data via a GET request to the /api/get-data endpoint. Please note that this code is for demonstration purposes only and has potential security issues that need to be addressed before deploying it in a production environment.
# Technologies
Built with Flask, Twilio, MailGun and Firebase Cloud Messaging
# Features
- The code sends SMS, Email and Firebase push notifications when it detects seizures, heart problem or fall.
- A REST API endpoint /api/send-data that accepts POST requests containing JSON data with a "BPM" key and stores the data in a global dictionary, app_data.
- A REST API endpoint /webhook/triggerd that accepts POST requests containing JSON data with a "message" key, checking the message key and sends appropriate notifications via SMS, email and Firebase push notifications.
- A REST API endpoint /api/get-data that returns the app_data dictionary in JSON format when accessed via a GET request.
- A home endpoint / that returns the string "ok" when accessed via a GET request.

> **IMPORTANT NOTE:This code contains many security issues such as there is no authentication or authorization, information disclosure, etc.** 
