# Introduction
This code is a Dart script that uses the Flutter framework to build a mobile application. The application fetches heart rate data from a remote server using the http package, and displays the data in the app's UI. The code also makes use of the Firebase messaging and notifications services to subscribe to a topic and receive push notifications, and the Flutter Local Notifications package to display notifications in the app. Additionally, it uses the Firebase_Core package to initialize the Firebase app. The code also uses the Timer package to periodically fetch the heart rate data from the server. This application is built to receive heart rate data and show notifications in an app environment.

# Technologies
Build with Flutter, Firebase Massaging , Flutter_local_notifications

# Features
- Fetching heart rate data from a remote server using HTTP GET requests.
- Parsing and displaying the fetched data in the app's UI.
- Subscribing to a topic and receiving push notifications through Firebase Cloud Messaging (FCM) service.
- Displaying notifications in the app using the Flutter Local Notifications package.
- Using Firebase Core package to initialize the Firebase app.
- Periodically fetching the heart rate data from the server using the Timer package.
- Handling background message when the app is closed
- Making use of Firebase messaging and notifications services to subscribe to a topic and receive push notifications.
- Using the http package to make HTTP requests to a remote server.
- Parsing JSON data and converting it into dart objects
- Using Dart's async/await features for asynchronous programming.
- Using the Flutter framework to build a mobile application with a responsive and dynamic UI.
- Using a timer to schedule repetitive tasks
All these features are working together to provide an efficient and robust way to build the application.

> **IMPORTANT NOTE: This code is for educational purposes and there are many security issues with it such as there is no encryption or authentication while data transferring and doesn't handling many cases like expired Firebase token, errors, invalid response, etc.
