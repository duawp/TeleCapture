
# 𝐓𝐞𝐥𝐞𝐂𝐚𝐩𝐭𝐮𝐫𝐞

<div align="center">
    <img src="https://github.com/user-attachments/assets/5a086c41-43c1-442e-b4da-5553a1876ded" alt="Telegram Logo" width="200"/>
</div>

### TeleCapture is a versatile tool designed to capture various forms of user activity and system data, including:

- **Screenshots**: Capture periodic snapshots of the user's screen.
- **Keylogging**: Log all keystrokes from the user's keyboard.
- **Webcam Snapshots**: Take snapshots from the user's camera at defined intervals.

This tool is perfect for monitoring and capturing data for research purposes, within legal and ethical boundaries.

## 𝙁𝙚𝙖𝙩𝙪𝙧𝙚𝙨

- **Screenshot Capture**: Automatically captures screenshots at regular intervals.
- **Keylogger**: Logs every keystroke and saves them for further analysis.
- **Webcam Snapshots**: Periodically captures images from the user's webcam.
- **Data Storage**: Stores captured data in a SQLite database.
- **Secure Communication**: Uses libcurl to send data over the internet.

## 𝙍𝙚𝙦𝙪𝙞𝙧𝙚𝙢𝙚𝙣𝙩𝙨

To use TeleCapture, you'll need the following libraries installed:

- **OpenCV**: Required for capturing screenshots and webcam images.
- **SQLite3**: For storing captured data locally in a database.
- **libcurl**: To securely send data over the network.
- **zlib**: Used for data compression.

## 𝙞𝙣𝙨𝙩𝙖𝙡𝙡𝙖𝙩𝙞𝙤𝙣

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/telecapture.git
    cd telecapture
    ```

2. Install the required dependencies:
    - For Ubuntu/Debian:
      ```bash
      sudo apt-get install libopencv-dev sqlite3 libcurl4-openssl-dev zlib1g-dev
      ```

    - For Fedora/RHEL:
      ```bash
      sudo dnf install opencv-devel sqlite sqlite-devel libcurl-devel zlib-devel
      ```

    - For Windows:
      - Download and install [OpenCV](https://opencv.org/releases/).
      - Install SQLite3 from [here](https://sqlite.org/download.html).
      - Install libcurl and zlib using your package manager or download pre-built binaries.

3. Build the project (if required):
    ```bash
    make
    ```

## Usage

Run the tool with the following command:

```bash
./telecapture





