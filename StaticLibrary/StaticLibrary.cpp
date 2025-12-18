#include <iostream>
#include <cstring>
#include <cstdlib>

extern "C" {
    int __stdcall outUint(unsigned int value)
    {
        std::cout << value;
        return 0;
    }

    int __stdcall outString(const char* ptr)
    {
        if (ptr == nullptr)
        {
            std::cout << std::endl;
            return 0;
        }
        for (int i = 0; ptr[i] != '\0'; i++)
            std::cout << ptr[i];
        return 0;
    }

    unsigned int __stdcall stringToUnsigned(char* buffer, const char* str)
    {
        if (str == nullptr || str[0] == '\0')
            return 0;

        unsigned int result = 0;
        int i = 0;

        while (str[i] == ' ' || str[i] == '\t') i++;

        if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
        {
            i += 2;
            while ((str[i] >= '0' && str[i] <= '9') ||
                (str[i] >= 'a' && str[i] <= 'f') ||
                (str[i] >= 'A' && str[i] <= 'F'))
            {
                result *= 16;
                if (str[i] >= '0' && str[i] <= '9')
                    result += str[i] - '0';
                else if (str[i] >= 'a' && str[i] <= 'f')
                    result += str[i] - 'a' + 10;
                else if (str[i] >= 'A' && str[i] <= 'F')
                    result += str[i] - 'A' + 10;
                i++;
            }
        }
        else
        {
            while (str[i] >= '0' && str[i] <= '9')
            {
                result = result * 10 + (str[i] - '0');
                i++;
            }
        }

        return result;
    }

    char* __stdcall unsignedToString(char* buffer, unsigned int value)
    {
        if (buffer == nullptr)
            return nullptr;

        if (value == 0)
        {
            buffer[0] = '0';
            buffer[1] = '\0';
            return buffer;
        }

        int i = 0;
        unsigned int temp = value;
        while (temp > 0)
        {
            buffer[i++] = '0' + (temp % 10);
            temp /= 10;
        }
        buffer[i] = '\0';

        int left = 0, right = i - 1;
        while (left < right)
        {
            char tmp = buffer[left];
            buffer[left] = buffer[right];
            buffer[right] = tmp;
            left++;
            right--;
        }

        return buffer;
    }

    void _pause()
    {
        system("pause");
    }

    unsigned int __stdcall stringLength(char*, const char* str)
    {
        if (str == nullptr) return 0;
        return static_cast<unsigned int>(std::strlen(str));
    }

    char* __stdcall stringConcat(char* buffer, char* str1, char* str2)
    {
        int i = 0;
        for (int j = 0; str1[j] != '\0'; j++)
        {
            if (i == 255)
                break;
            buffer[i++] = str1[j];
        }
        for (int j = 0; str2[j] != '\0'; j++)
        {
            if (i == 255)
                break;
            buffer[i++] = str2[j];
        }
        buffer[i] = '\0';
        return buffer;
    }
}