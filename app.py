from flask import Flask,jsonify,request,abort,render_template
import send_email
import send_sms
from firebase_send import send_topic_push
app = Flask(__name__)

app_data={
    "heart_rate":0,
}


@app.route("/")
def home():
    return 'ok'

@app.route("/api/send-data",methods=["POST"])
def send_data():
    if request.method == 'POST':
        request_data = request.get_json()
        app_data["heart_rate"]=request_data["BPM"]
        return {"success":"ok"},200
    else:
        abort(400)

@app.route('/webhook/triggerd', methods=['POST'])
def webhook():
    if request.method == 'POST':
        request_data = request.get_json()
        if request_data['message']== "Seizure Detected!":
            send_sms.send_message('Seizure Detected!','+0123456789')
            send_email.send_email_message("email@gmail.com","Seizure Detected!","Seizure Detected!")
            send_topic_push("Seizure Detected")
        elif request_data['message'] =="Heart Problem!":
            send_sms.send_message('Heart Problem!','+0123456789')
            send_email.send_email_message("email@gmail.com","Heart Problem!","Heart Problem!")
            send_topic_push("Heart Problem")
        if request_data['message'] =="FALL DETECTED!":
            send_sms.send_message('FALL DETECTED!','+0123456789')
            send_email.send_email_message("email@gmail.com","FALL DETECTED!","FALL DETECTED!")
            send_topic_push("Fall Detected!")

        return {"success":'ok'},200
    else:
        abort(400)



@app.route("/api/get-data",methods=["GET"])
def get_data():
    if request.method == 'GET':
        return jsonify(app_data),200
    else:
        abort(400)


if __name__ == "__main__":
    app.run("0.0.0.0",port=5000,debug=True)
