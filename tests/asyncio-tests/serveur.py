#!/usr/bin/env python3.8
import asyncio


def encodeBuffer(message):
    sizeBuffer_send = len(message).to_bytes(4, "big")
    print("\033[91mSize Buffer sent : ", int.from_bytes(sizeBuffer_send, "big"), "\033[0m")
    
    return sizeBuffer_send

def decodeBuffer(buffer):
    sizeBuffer_received = int.from_bytes(buffer, "big")
    print(f"\033[36mReceived size buffer : {sizeBuffer_received!r}\033[0m")
    
    return sizeBuffer_received

async def encode_file(name):
    try:
        with open("../Simulation_BD/"+name) as file:
            data = file.read()
    except IOError:
        print("Erreur ! Le fichier n'a pas pu être ouvert")
        data = "Erreur ! Le fichier n'a pas pu être ouvert"
    
    return data

async def download_file(data):
    with open("../Simulation_BD/FileTest.py", "w") as file:
        file.write(data)

async def send_toClient(writer, message):
    sizeBuffer_send = encodeBuffer(message)
    writer.write(sizeBuffer_send)
    # print(f'\033[36mMessage send : \033[93m{message}\033[0m')
    writer.write(message.encode())

async def receive_fromClient(reader):
    sizeBuffer_received = decodeBuffer(await reader.read(4))
    data = await reader.read(sizeBuffer_received)

    return data.decode()

async def handle_echo(reader, writer):
    connexion = True
    while connexion:
        action = await reader.read(1)
        action = int.from_bytes(action, "big")
        if action == 0:
            data = await receive_fromClient(reader)
            await download_file(data)
        elif action == 1:
            nameFile = await receive_fromClient(reader)
            await send_toClient(writer, await encode_file(nameFile))
        elif action == 2:
            connexion = False
        else:
            print("Error action.")

        await writer.drain()
    print("Close the connection")
    writer.close()

async def main():
    server = await asyncio.start_server(
        handle_echo, '192.168.0.16', 8888)

    addr = server.sockets[0].getsockname()
    print(f'Serving on {addr}')

    async with server:
        await server.serve_forever()

asyncio.run(main())
