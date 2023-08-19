from twilio.rest import Client
import pandas

def send_notification(crop_details):

    account_sid = "AC8ca3e69dc744bf8951afb2a532a4f20f"
    auth_token  = "e8e42ff804f1e4b74413c5c06e535cf4"

    client = Client(account_sid, auth_token)

    df = pandas.read_csv("user_data.csv")

    mobile_nos = list(df['Mobile Number'].dropna())

    mobile_nos = ['+91'+str(int(i)) for i in mobile_nos]

    for number in mobile_nos:
        message = client.messages.create(to=number, from_="+12765304920",body=crop_details)
        print("Msg sent to",number)

