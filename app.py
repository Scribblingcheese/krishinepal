import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
import crop_recomendation
import rainfall_prediction
import sms
from datetime import date
import warnings
import threading
import location
from flask import Flask, render_template, url_for, make_response
import json
import time
from flask import jsonify

warnings.filterwarnings("ignore")

op_string = "No data"
state = ""
op_data = None
count = 0

def update_value():
    global op_string
    global op_data
    global state
    global count

    month_to_no = {1:'JAN', 2:'FEB', 3:'MAR', 4:'APR', 5:'MAY', 6:'JUN', 7:'JUL', 8:'AUG', 9:'SEP', 10:'OCT', 11:'NOV', 12:'DEC'}

    cred = credentials.Certificate('testing64jc-firebase-adminsdk-lz1q6-8a9c3c25c2.json')
    firebase_admin.initialize_app(cred, {'databaseURL': "https://testing64jc-default-rtdb.firebaseio.com"})
    firebase_admin.get_app()

    while True:
        ref = db.reference('/data')
        received_data = ref.get()
        op_data = received_data

        t = date.today()
        m = t.month
        month = month_to_no[m]
        state = location.get_state()
        state = state.upper()

        rainfall_value = rainfall_prediction.get_rainfall_prediction(state, month)

        op_string = crop_recomendation.make_prediction([received_data['N'], received_data['P'], received_data['K'],
                                                        received_data['temperature'], received_data['humidity'],
                                                        received_data['ph'], rainfall_value])

        print("Data is updated")
        print(op_string)

        if count == 0:
            count += 1
            send_messages()
            print("Messages are sent.")

        time.sleep(10)  # Data gets updated after 10 sec


app = Flask(__name__, static_folder="C:/Users/JAYA CHANDRA/Desktop/SCS Project/Code/static")

@app.route('/')
def func():
    return render_template('home.html')

@app.route('/crop_recommendation', methods=["GET", "POST"])
def hello_world():
    while op_string == "No data":
        pass

    return render_template("recommendation.html", op_string=op_string, loc=state.lower().capitalize())

@app.route('/home')
def home():
    return render_template('home.html')

@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/graph', methods=["GET", "POST"])
def main():
    return render_template('index.html')

@app.route("/crop_graph", methods=["GET", "POST"])
def crop_dashboard():
    global op_string

    temp = {}

    while op_string == "No data":
        pass

    for s in op_string:
        at = s.find(":")
        crop_name = str(s[:at])
        percentage = float(s[at+1:-2])

        temp[crop_name] = percentage

    crop_array = list(temp.keys())
    percentage_array = list(temp.values())

    return render_template("sample.html", labels=crop_array, values=percentage_array)


def send_messages():
    global op_string

    msg = "Crop recommendations in your location are:  "

    while op_string == "No data":
        pass

    for crop in op_string:
        msg = msg + str(crop) + " , "

    msg = msg[:-2]
    msg = msg + "."

    print(msg)

    sms.send_notification(msg)


@app.route('/data', methods=["GET", "POST"])
def data():
    global op_data

    while op_data is None:
        pass

    Temperature = op_data['temperature']
    Humidity = op_data['humidity']
    ph = op_data['ph']
    N = op_data['N']
    P = op_data['P']
    K = op_data['K']

    print(Temperature)
    print(Humidity)
    print(ph)
    print(N)
    print(P)
    print(K)

    data = [time.time() * 1000, Temperature, Humidity, ph, N, P, K]

    response = make_response(json.dumps(data))

    response.content_type = 'application/json'

    return response

@app.route('/crop_data', methods=["GET", "POST"])
def crop_data():
    global op_string

    while op_string == "No data":
        pass

    temp = {}

    for s in op_string:
        at = s.find(":")
        crop_name = str(s[:at])
        percentage = float(s[at+1:-2])

        temp[crop_name] = percentage

    crop_array = list(temp.keys())
    percentage_array = list(temp.values())

    other = [crop_array, percentage_array]
    print(other)

    response = make_response(json.dumps(other))

    response.content_type = 'application/json'

    return response

t = threading.Thread(target=update_value)
t.start()

if __name__ == '__main__':
    app.run()
