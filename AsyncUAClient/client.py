import requests
import asyncio
from asyncua import Client
from dotenv import load_dotenv
import os
import time

load_dotenv()

AUTH_TOKEN = os.getenv("AUTH_TOKEN")
USER_ID = os.getenv("USER_ID")

def send_msg(message):
    send_text = f"https://api.telegram.org/bot{AUTH_TOKEN}/sendMessage?chat_id={USER_ID}&parse_mode=Markdown&text={message}"
    response = requests.get(send_text)
    return response.json()

def get_last_message():
    get_last_message = f"https://api.telegram.org/bot{AUTH_TOKEN}/getUpdates?chat_id={USER_ID}"
    response = requests.get(get_last_message).json()["result"][-1]
    return response

url = "opc.tcp://localhost:4845/server"
namespace = "http://iot.opcualex.de"

async def main():
    print(f"Connecting to {url} ...")
    time.sleep(20)
    send_msg("I am online for you!.")
    async with Client(url=url) as client:
        nsidx = await client.get_namespace_index(namespace)
        last_replied = 0
        while True:
            try:
                response = get_last_message()
                res = response["message"]["text"].lower()
                if int(time.time()) - response["message"]["date"] < 25 and response["message"]["message_id"] != last_replied:
                    if res in ["hi", "hello"]:
                        send_msg("Hi, how are you? I can only inform you about the temperature. Please send me the 'temperature' command.")
                    elif  res == "temperature":
                        res = await client.nodes.objects.call_method(f"{nsidx}:get temperature")
                        send_msg(f"Your living room temperature is {res.strip('0')}0°C.")
                    else:
                        send_msg("I didn't understand! Please send hi.")
                    last_replied = response["message"]["message_id"]
                time.sleep(5)
            except Exception as err:
                print(err)
                continue

if __name__ == "__main__":
    asyncio.run(main())
