import requests
import json

url = "http://192.168.1.81:9000/k3cloud/services/k3STUservice.asmx/JudgeSpecialUserOnline"

response = requests.request("GET", url, data="", headers="")
data     = json.loads(response.text)
print(data['Result'])