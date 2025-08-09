#pragma once


#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>
#include <string>


namespace ienium::utils
{
    class IeniumPipeServer
    {
        private:
        HANDLE hPipe;
        std::string pipeName;
        const int pipeType = PIPE_TYPE_MESSAGE;     // String data is sent
        const int pipeMode = PIPE_WAIT;             // BLocking mode
        const DWORD bufferSize;
        const DWORD MAX_BUFFER_SIZE = 1024 * 1024;  // 1MB

        std::atomic<bool> running;
        std::function<void (std::string)> processMessageCallback;
        std::function<void (bool)> connectionStatusChangedCallback;

        std::thread listenerThread;

        public:
        IeniumPipeServer (const std::string& name, const DWORD buffer_size) : pipeName ("\\\\.\\pipe\\" + name), bufferSize (buffer_size), hPipe (INVALID_HANDLE_VALUE) {}
        
        bool StartServer ()
        {
            hPipe = CreateNamedPipe (
                pipeName.c_str (),
                PIPE_ACCESS_INBOUND,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
                PIPE_UNLIMITED_INSTANCES,
                bufferSize,
                bufferSize,
                5000,
                0 
            );

            if (hPipe == INVALID_HANDLE_VALUE)
            {
                std::cerr << "Failed to create named Pipe. Error: " << GetLastError () << std::endl;
                return false;
            }

            std::cout << "Named pipe server created: " << pipeName << std::endl;
            running.store (true);
            return true;
        }

        void Shutdown ()
        {
            if (!running.load ())
            {
                return;
            }

            std::cout << "Initiating shutdown..." << std::endl;
            running.store (false);

            if (hPipe != INVALID_HANDLE_VALUE)
            {
                CancelIoEx (hPipe, NULL);

                DisconnectNamedPipe(hPipe);

                Sleep (100);

                CloseHandle (hPipe);

                hPipe = INVALID_HANDLE_VALUE;
            }
            
            if (listenerThread.joinable ())
            {
                listenerThread.join ();
            }

            std::cout << "Shutdown complete" << std::endl;
        }

        void SetProcessMessageCallback (std::function<void (std::string)> cb)
        {
            processMessageCallback = cb;
        }

        void SetConnectionStatusChangedCallBack (std::function<void (bool)> cb)
        {
            connectionStatusChangedCallback = cb;
        }

        void StartListening ()
        {
            listenerThread = std::thread (
                [this] () { this->ListenForMessages(); }
            );
        }
        private:
        void ListenForMessages ()
        {
            while (running)
            {
                std::cout << "Waiting for client connection" << std::endl;
                

                bool connected = ConnectNamedPipe (hPipe, NULL) ? TRUE : (GetLastError () == ERROR_PIPE_CONNECTED);

                if (connected)
                {
                    std::cout << "Client connected" << std::endl;
                    if (connectionStatusChangedCallback != NULL)
                        connectionStatusChangedCallback (true);

                    while (running.load ())
                    {
                        std::string full_message = ReadFullMessage ();

                        if (!full_message.empty ())
                        {
                            if (processMessageCallback != NULL)
                                processMessageCallback (full_message);
                            else
                                std::cout << "ProcessMessageCallback function not set. Message is not being processed!" << std::endl;
                        }
                        else
                        {
                            DWORD error = GetLastError ();
                            if (error == ERROR_BROKEN_PIPE)
                            {
                                std::cout << "Client disconnected." << std::endl;
                                break;
                            }
                            else if (error != ERROR_SUCCESS)
                            {
                                std::cerr << "Read failed. Error: " << error << std::endl;
                                break;
                            }
                        }
                    }
                    DisconnectNamedPipe (hPipe);
                    if (connectionStatusChangedCallback != NULL)
                        connectionStatusChangedCallback (false);
                }
                else
                {
                    std::cerr << "ConnectNamedPipeFailed. Error: " << GetLastError () << std::endl;
                    break;
                }
            }
        }

        
        std::string ReadFullMessage ()
        {
            std::vector<char> buffer (bufferSize);
            std::string full_message;
            DWORD bytes_read;
            DWORD total_bytes_avail;
            DWORD bytes_left_this_message;

            bool peek_success = PeekNamedPipe (
                hPipe,
                NULL,
                0,
                NULL,
                &total_bytes_avail,
                &bytes_left_this_message
            );

            if (!peek_success)
            {
                return "";
            }
            SetLastError(0);

            if (total_bytes_avail == 0)
            {
                //std::cout << "Message is empty" << std::endl;
                return "";
            }

            if (bytes_left_this_message > buffer.size ())
            {
                if (bytes_left_this_message > MAX_BUFFER_SIZE)
                {
                    std::cerr << "Message too large: " << bytes_left_this_message << " bytes. Discarding message" << std::endl;
                    std::vector<char> discardBuffer (bufferSize);
                    DWORD total_discarded = 0;

                    while (total_discarded < bytes_left_this_message)
                    {
                        DWORD to_read = (discardBuffer.size () < (bytes_left_this_message - total_discarded)) ? discardBuffer.size () : (bytes_left_this_message - total_discarded); 

                        bool success = ReadFile (
                            hPipe,
                            discardBuffer.data (),
                            to_read,
                            &bytes_read,
                            NULL
                        );
                        if (!success || bytes_read == 0)
                        {
                            break;
                        }
                        total_discarded += bytes_read;
                    }
                    return "";
                }
                buffer.resize (bytes_left_this_message);
            }

            bool success = ReadFile (
                hPipe,
                buffer.data (),
                static_cast<DWORD>(buffer.size ()),
                &bytes_read,
                NULL
            );

            if (success && bytes_read > 0)
            {
                return std::string (buffer.data (), bytes_read);
            }
            return "";
        }
    };
}