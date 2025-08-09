#pragma once


#include <windows.h>
#include <iostream>
#include <string>



class IeniumPipeClient
{
    private:
    HANDLE hPipe;
    std::string pipeName;
    bool connected = false;

    public:
    IeniumPipeClient (const std::string& name) : pipeName ("\\\\.\\pipe\\" + name) {}
    virtual ~IeniumPipeClient ()
    {
        Disconnect ();
    }

    bool Connect (int timeout_ms = 5000)
    {
        hPipe = CreateFile (
            pipeName.c_str (),
            GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE)
        {
            connected = true;
            return true;
        }

        DWORD error = GetLastError ();
        if (error == ERROR_PIPE_BUSY)
        {
            if (WaitNamedPipe (pipeName.c_str (), timeout_ms))
            {
                return Connect(0);
            }
        }

        std::cerr << "Failed to connect to pipe: " << pipeName << " Error: " << error << std::endl;
        return false;
    }

    void Disconnect ()
    {
        if (hPipe != INVALID_HANDLE_VALUE)
        {
            CloseHandle (hPipe);
            hPipe = INVALID_HANDLE_VALUE;
        }

        connected = false;
    }

    bool SendData (const std::string& message)
    {
        if (!connected || hPipe == INVALID_HANDLE_VALUE)
        {
            connected = false;
            std::cerr << "Not connected to pipe." << std::endl;
            return false;
        }

        DWORD bytes_written;
        bool sucess = WriteFile (
            hPipe,
            message.c_str (),
            static_cast<DWORD>(message.length ()),
            &bytes_written,
            NULL
        );

        if (!sucess)
        {
            DWORD error = GetLastError ();
            std::cerr << "Failed to write to pipe. Error: " << error << std::endl;

            if (error == ERROR_BROKEN_PIPE || ERROR_NO_DATA)
            {
                connected = false;
                CloseHandle (hPipe);
                hPipe = INVALID_HANDLE_VALUE;
            }
            return false;
        }

        FlushFileBuffers (hPipe);
        return true;
    }

    bool IsConnected () const
    {
        return connected && hPipe != INVALID_HANDLE_VALUE;
    }
};