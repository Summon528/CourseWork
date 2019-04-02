import urllib.request


def main():
    while True:
        # Try connect to the URL
        try:
            url = input("URL: ")
            res = urllib.request.urlopen(url)
            break
        except (ValueError, urllib.error.URLError):
            print("invalid rul")
    # Read response
    res = res.read().decode()
    # Print first 3000 chars
    print(res[:3000])
    # Print length
    print(len(res))


if __name__ == "__main__":
    main()
