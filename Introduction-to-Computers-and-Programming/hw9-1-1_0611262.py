import socket
from urllib.parse import urlsplit


def main():
    while True:
        # Try connect to the given url
        try:
            _, netloc, path, query, _ = urlsplit(input("URL: "))
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((netloc, 80))
            break
        except (socket.gaierror, OSError):
            print("invalid url")
    # If path is not given, we assume it is /
    if path == "":
        path = "/"
    s.send(("GET {} HTTP/1.1\r\n".format(path+"?"+query) +
            "Host:{}\r\n".format(netloc) +
            "Connection:Close\r\n\r\n").encode())
    # Collect Response
    response = b''
    while True:
        recv = s.recv(1024)
        if not recv:
            break
        response += recv
    response = response.decode()
    # Print first 3000 chars
    print(response[:3000])
    # Print document length
    print(len(response) - response.find("\r\n\r\n") - 4)


if __name__ == "__main__":
    main()
