import json

with open('user.json' , 'r') as f:
    user_dict = json.load(f)

#print(type(user_dict))

username = "winnie"
password = "1230"
if username in user_dict:
    print(user_dict[username])
    if user_dict[username] == password:
        print("verified")
