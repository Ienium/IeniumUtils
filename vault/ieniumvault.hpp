#pragma once

#include <windows.h>
#include <wincred.h>
#include <string>
#include <iostream>

#pragma comment(lib, "advapi32.lib")

class IeniumVault
{
    private:
    const std::wstring appName;

    public:

    IeniumVault (const std::wstring& application_name) : appName (application_name) {}

    bool StoreCredentials (const std::wstring credentials_name, const std::string& credentials)
    {
        const std::wstring target_name = std::wstring (appName + L"_" + credentials_name);
        return StoreCredentialsInternal (target_name, credentials);
    }

    std::string GetCredentials (const std::wstring credentials_name)
    {
        std::wstring target_name = std::wstring (appName +  L"_" + credentials_name);
        return GetCredentialsInternal (target_name);
    }

    bool DeleteCredentials (const std::wstring credentials_name)
    {
        std::wstring target_name = std::wstring (appName +  L"_" + credentials_name);
        return DeleteCredentialsInternal (target_name);
    }


    private:
    bool StoreCredentialsInternal (const std::wstring& target_name, const std::string& credentials)
    {
        CREDENTIALW cred = {};
        cred.Type = CRED_TYPE_GENERIC;
        cred.TargetName = const_cast<LPWSTR> (target_name.c_str ());
        cred.CredentialBlobSize = static_cast<DWORD> (credentials.length ());
        cred.CredentialBlob = reinterpret_cast<LPBYTE> (const_cast<char*> (credentials.c_str ()));
        cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
        cred.UserName = const_cast<LPWSTR> (L"CredentialsUser");

        BOOL result = CredWriteW (&cred, 0);
        
        if (!result)
        {
            std::wcerr << L"Failed to store credential. Error: " << GetLastError () << std::endl;
            return false;
        }

        return true;
    }

    std::string GetCredentialsInternal (const std::wstring& target_name)
    {
        PCREDENTIALW pcred = nullptr;
        BOOL result = CredReadW (target_name.c_str (), CRED_TYPE_GENERIC, 0, &pcred);

        if (!result)
        {
            DWORD error = GetLastError ();
            if (error != ERROR_NOT_FOUND)
            {
                std::wcerr << L"Failed to read credential. Error: " << error << std::endl;
            }
            return "";
        }

        std::string credentials (reinterpret_cast<char*> (pcred->CredentialBlob), pcred->CredentialBlobSize);
        CredFree (pcred);
        return credentials;
    }

    bool DeleteCredentialsInternal (const std::wstring& target_name)
    {
        BOOL result = CredDeleteW(target_name.c_str(), CRED_TYPE_GENERIC, 0);

        if (!result)
        {
            DWORD error = GetLastError ();
            if (error != ERROR_NOT_FOUND)
            {
                std::wcerr << L"Failed to read credential. Error: " << error << std::endl;
            }
            return false;
        }

        return true;
    }    
};