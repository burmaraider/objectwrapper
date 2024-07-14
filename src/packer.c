#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/ltdefs.h"

// format is as follows:
//  4 bytes for the number of class definitions
//  for each class definition:
//  4 bytes for the length of the class name
//  n bytes for the class name
//  4 bytes for the length of the parent class name
//  n bytes for the parent class name
//  4 bytes for the number of properties
//  for each property:
//  4 bytes for the length of the property name
//  n bytes for the property name
//  4 bytes for the property type
//  4 bytes for the property flags
//  4 bytes for the length of the default value string
//  n bytes for the default value string
//  4 bytes for the default value float
//  4 bytes for the default value vector x
//  4 bytes for the default value vector y
//  4 bytes for the default value vector z

int SetupByteBufferLT1(int nDefs, void **classDefs, char **outputBuffer)
{
    int bufferSize = 4; // number of class definitions
    for (int i = 0; i < nDefs; i++)
    {
        bufferSize += 4;                                                   // length of class name
        bufferSize += strlen(((ClassDef_LT1 *)classDefs[i])->m_ClassName); // class name
        bufferSize += 4;                                                   // number of properties
        bufferSize += 4;                                                   // length of parent class name
        if (((ClassDef_LT1 *)classDefs[i])->m_ParentClass != NULL)
        {
            bufferSize += strlen(((ClassDef_LT1 *)classDefs[i])->m_ParentClass->m_ClassName); // parent class name
        }
        for (int j = 0; j < ((ClassDef_LT1 *)classDefs[i])->m_nProps; j++)
        {
            bufferSize += 4;                                                             // length of property name
            bufferSize += strlen(((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_PropName); // property name
            bufferSize += 4;                                                             // property type
            bufferSize += 4;                                                             // property flags
            bufferSize += 4;                                                             // length of default value string
            if (((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueString != NULL)
            {
                bufferSize += strlen(((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueString); // default value string
            }
            bufferSize += 4; // default value float
            bufferSize += 4; // default value vector x
            bufferSize += 4; // default value vector y
            bufferSize += 4; // default value vector z
        }
    }

    *outputBuffer = (char *)malloc(bufferSize);
    if (outputBuffer == NULL)
    {
        printf("Failed to allocate memory for byte buffer\n");
        return 0;
    }

    char *bufferPtr = *outputBuffer;
    memcpy(bufferPtr, &nDefs, 4);
    bufferPtr += 4;

    for (int i = 0; i < nDefs; i++)
    {
        int classNameLength = strlen(((ClassDef_LT1 *)classDefs[i])->m_ClassName);
        memcpy(bufferPtr, &classNameLength, 4);
        bufferPtr += 4;
        memcpy(bufferPtr, ((ClassDef_LT1 *)classDefs[i])->m_ClassName, classNameLength);
        bufferPtr += classNameLength;

        const char *parentClassString = ((ClassDef_LT1 *)classDefs[i])->m_ParentClass ? ((ClassDef_LT1 *)classDefs[i])->m_ParentClass->m_ClassName : NULL;
        int parentClassStringLength = (parentClassString != NULL) ? strlen(parentClassString) : 0;

        memcpy(bufferPtr, &parentClassStringLength, 4);
        bufferPtr += 4;

        if (parentClassString != NULL)
        {
            memcpy(bufferPtr, parentClassString, parentClassStringLength);
            bufferPtr += parentClassStringLength;
        }

        memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_nProps, 4);
        bufferPtr += 4;

        for (int j = 0; j < ((ClassDef_LT1 *)classDefs[i])->m_nProps; j++)
        {
            int propNameLength = strlen(((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_PropName);
            memcpy(bufferPtr, &propNameLength, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, ((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_PropName, propNameLength);
            bufferPtr += propNameLength;

            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_PropType, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_PropFlags, 4);
            bufferPtr += 4;

            const char *defaultValueString = ((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueString;
            int defaultValueStringLength = (defaultValueString != NULL) ? strlen(defaultValueString) : 0;

            memcpy(bufferPtr, &defaultValueStringLength, 4);
            bufferPtr += 4;

            if (defaultValueString != NULL)
            {
                memcpy(bufferPtr, defaultValueString, defaultValueStringLength);
                bufferPtr += defaultValueStringLength;
            }

            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueFloat, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueVector.x, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueVector.y, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_LT1 *)classDefs[i])->m_Props[j].m_DefaultValueVector.z, 4);
            bufferPtr += 4;
        }
    }
    return bufferSize;
}

int SetupByteBufferLT2(int nDefs, void **classDefs, char **outputBuffer)
{
    int bufferSize = 4; // number of class definitions
    for (int i = 0; i < nDefs; i++)
    {
        bufferSize += 4;                                                   // length of class name
        bufferSize += strlen(((ClassDef_LT2 *)classDefs[i])->m_ClassName); // class name
        bufferSize += 4;                                                   // number of properties
        bufferSize += 4;                                                   // length of parent class name
        if (((ClassDef_LT2 *)classDefs[i])->m_ParentClass != NULL)
        {
            bufferSize += strlen(((ClassDef_LT2 *)classDefs[i])->m_ParentClass->m_ClassName); // parent class name
        }
        for (int j = 0; j < ((ClassDef_LT2 *)classDefs[i])->m_nProps; j++)
        {
            bufferSize += 4;                                                             // length of property name
            bufferSize += strlen(((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_PropName); // property name
            bufferSize += 4;                                                             // property type
            bufferSize += 4;                                                             // property flags
            bufferSize += 4;                                                             // length of default value string
            if (((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueString != NULL)
            {
                bufferSize += strlen(((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueString); // default value string
            }
            bufferSize += 4; // default value float
            bufferSize += 4; // default value vector x
            bufferSize += 4; // default value vector y
            bufferSize += 4; // default value vector z
        }
    }

    *outputBuffer = (char *)malloc(bufferSize);
    if (outputBuffer == NULL)
    {
        printf("Failed to allocate memory for byte buffer\n");
        return 0;
    }

    char *bufferPtr = *outputBuffer;
    memcpy(bufferPtr, &nDefs, 4);
    bufferPtr += 4;

    for (int i = 0; i < nDefs; i++)
    {
        int classNameLength = strlen(((ClassDef_LT2 *)classDefs[i])->m_ClassName);
        memcpy(bufferPtr, &classNameLength, 4);
        bufferPtr += 4;
        memcpy(bufferPtr, ((ClassDef_LT2 *)classDefs[i])->m_ClassName, classNameLength);
        bufferPtr += classNameLength;

        const char *parentClassString = ((ClassDef_LT2 *)classDefs[i])->m_ParentClass ? ((ClassDef_LT2 *)classDefs[i])->m_ParentClass->m_ClassName : NULL;
        int parentClassStringLength = (parentClassString != NULL) ? strlen(parentClassString) : 0;

        memcpy(bufferPtr, &parentClassStringLength, 4);
        bufferPtr += 4;

        if (parentClassString != NULL)
        {
            memcpy(bufferPtr, parentClassString, parentClassStringLength);
            bufferPtr += parentClassStringLength;
        }

        memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_nProps, 4);
        bufferPtr += 4;

        for (int j = 0; j < ((ClassDef_LT2 *)classDefs[i])->m_nProps; j++)
        {
            int propNameLength = strlen(((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_PropName);
            memcpy(bufferPtr, &propNameLength, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, ((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_PropName, propNameLength);
            bufferPtr += propNameLength;

            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_PropType, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_PropFlags, 4);
            bufferPtr += 4;

            const char *defaultValueString = ((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueString;
            int defaultValueStringLength = (defaultValueString != NULL) ? strlen(defaultValueString) : 0;

            memcpy(bufferPtr, &defaultValueStringLength, 4);
            bufferPtr += 4;

            if (defaultValueString != NULL)
            {
                memcpy(bufferPtr, defaultValueString, defaultValueStringLength);
                bufferPtr += defaultValueStringLength;
            }

            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueFloat, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueVector.x, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueVector.y, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_LT2 *)classDefs[i])->m_Props[j].m_DefaultValueVector.z, 4);
            bufferPtr += 4;
        }
    }
    return bufferSize;
}
// Setup the byte buffer from class definitions
int SetupByteBufferTalon(int nDefs, void **classDefs, char **outputBuffer)
{
    int bufferSize = 4; // number of class definitions
    for (int i = 0; i < nDefs; i++)
    {
        bufferSize += 4;                                                     // length of class name
        bufferSize += strlen(((ClassDef_Talon *)classDefs[i])->m_ClassName); // class name
        bufferSize += 4;                                                     // number of properties
        bufferSize += 4;                                                     // length of parent class name
        if (((ClassDef_Talon *)classDefs[i])->m_ParentClass != NULL)
        {
            bufferSize += strlen(((ClassDef_Talon *)classDefs[i])->m_ParentClass->m_ClassName); // parent class name
        }
        for (int j = 0; j < ((ClassDef_Talon *)classDefs[i])->m_nProps; j++)
        {
            bufferSize += 4;                                                               // length of property name
            bufferSize += strlen(((ClassDef_Talon *)classDefs[i])->m_Props[j].m_PropName); // property name
            bufferSize += 4;                                                               // property type
            bufferSize += 4;                                                               // property flags
            bufferSize += 4;                                                               // length of default value string
            if (((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueString != NULL)
            {
                bufferSize += strlen(((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueString); // default value string
            }
            bufferSize += 4; // default value float
            bufferSize += 4; // default value vector x
            bufferSize += 4; // default value vector y
            bufferSize += 4; // default value vector z
        }
    }

    *outputBuffer = (char *)malloc(bufferSize);
    if (outputBuffer == NULL)
    {
        printf("Failed to allocate memory for byte buffer\n");
        return 0;
    }

    char *bufferPtr = *outputBuffer;
    memcpy(bufferPtr, &nDefs, 4);
    bufferPtr += 4;

    for (int i = 0; i < nDefs; i++)
    {
        int classNameLength = strlen(((ClassDef_Talon *)classDefs[i])->m_ClassName);
        memcpy(bufferPtr, &classNameLength, 4);
        bufferPtr += 4;
        memcpy(bufferPtr, ((ClassDef_Talon *)classDefs[i])->m_ClassName, classNameLength);
        bufferPtr += classNameLength;

        const char *parentClassString = ((ClassDef_Talon *)classDefs[i])->m_ParentClass ? ((ClassDef_Talon *)classDefs[i])->m_ParentClass->m_ClassName : NULL;
        int parentClassStringLength = (parentClassString != NULL) ? strlen(parentClassString) : 0;

        memcpy(bufferPtr, &parentClassStringLength, 4);
        bufferPtr += 4;

        if (parentClassString != NULL)
        {
            memcpy(bufferPtr, parentClassString, parentClassStringLength);
            bufferPtr += parentClassStringLength;
        }

        memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_nProps, 4);
        bufferPtr += 4;

        for (int j = 0; j < ((ClassDef_Talon *)classDefs[i])->m_nProps; j++)
        {
            int propNameLength = strlen(((ClassDef_Talon *)classDefs[i])->m_Props[j].m_PropName);
            memcpy(bufferPtr, &propNameLength, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, ((ClassDef_Talon *)classDefs[i])->m_Props[j].m_PropName, propNameLength);
            bufferPtr += propNameLength;

            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_PropType, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_PropFlags, 4);
            bufferPtr += 4;

            const char *defaultValueString = ((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueString;
            int defaultValueStringLength = (defaultValueString != NULL) ? strlen(defaultValueString) : 0;

            memcpy(bufferPtr, &defaultValueStringLength, 4);
            bufferPtr += 4;

            if (defaultValueString != NULL)
            {
                memcpy(bufferPtr, defaultValueString, defaultValueStringLength);
                bufferPtr += defaultValueStringLength;
            }

            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueFloat, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueVector.x, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueVector.y, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_Talon *)classDefs[i])->m_Props[j].m_DefaultValueVector.z, 4);
            bufferPtr += 4;
        }
    }

    return bufferSize;
}

int SetupByteBufferJupiter(int nDefs, void **classDefs, char **outputBuffer)
{
    int bufferSize = 4; // number of class definitions
    for (int i = 0; i < nDefs; i++)
    {
        bufferSize += 4;                                                       // length of class name
        bufferSize += strlen(((ClassDef_Jupiter *)classDefs[i])->m_ClassName); // class name
        bufferSize += 4;                                                       // number of properties
        bufferSize += 4;                                                       // length of parent class name
        if (((ClassDef_Jupiter *)classDefs[i])->m_ParentClass != NULL)
        {
            bufferSize += strlen(((ClassDef_Jupiter *)classDefs[i])->m_ParentClass->m_ClassName); // parent class name
        }
        for (int j = 0; j < ((ClassDef_Jupiter *)classDefs[i])->m_nProps; j++)
        {
            bufferSize += 4;                                                                 // length of property name
            bufferSize += strlen(((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_PropName); // property name
            bufferSize += 4;                                                                 // property type
            bufferSize += 4;                                                                 // property flags
            bufferSize += 4;                                                                 // length of default value string
            if (((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueString != NULL)
            {
                bufferSize += strlen(((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueString); // default value string
            }
            bufferSize += 4; // default value float
            bufferSize += 4; // default value vector x
            bufferSize += 4; // default value vector y
            bufferSize += 4; // default value vector z
        }
    }

    *outputBuffer = (char *)malloc(bufferSize);
    if (outputBuffer == NULL)
    {
        printf("Failed to allocate memory for byte buffer\n");
        return 0;
    }

    char *bufferPtr = *outputBuffer;
    memcpy(bufferPtr, &nDefs, 4);
    bufferPtr += 4;

    for (int i = 0; i < nDefs; i++)
    {
        int classNameLength = strlen(((ClassDef_Jupiter *)classDefs[i])->m_ClassName);
        memcpy(bufferPtr, &classNameLength, 4);
        bufferPtr += 4;
        memcpy(bufferPtr, ((ClassDef_Jupiter *)classDefs[i])->m_ClassName, classNameLength);
        bufferPtr += classNameLength;

        const char *parentClassString = ((ClassDef_Jupiter *)classDefs[i])->m_ParentClass ? ((ClassDef_Jupiter *)classDefs[i])->m_ParentClass->m_ClassName : NULL;
        int parentClassStringLength = (parentClassString != NULL) ? strlen(parentClassString) : 0;

        memcpy(bufferPtr, &parentClassStringLength, 4);
        bufferPtr += 4;

        if (parentClassString != NULL)
        {
            memcpy(bufferPtr, parentClassString, parentClassStringLength);
            bufferPtr += parentClassStringLength;
        }

        memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_nProps, 4);
        bufferPtr += 4;

        for (int j = 0; j < ((ClassDef_Jupiter *)classDefs[i])->m_nProps; j++)
        {
            int propNameLength = strlen(((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_PropName);
            memcpy(bufferPtr, &propNameLength, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, ((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_PropName, propNameLength);
            bufferPtr += propNameLength;

            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_PropType, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_PropFlags, 4);
            bufferPtr += 4;

            const char *defaultValueString = ((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueString;
            int defaultValueStringLength = (defaultValueString != NULL) ? strlen(defaultValueString) : 0;

            memcpy(bufferPtr, &defaultValueStringLength, 4);
            bufferPtr += 4;

            if (defaultValueString != NULL)
            {
                memcpy(bufferPtr, defaultValueString, defaultValueStringLength);
                bufferPtr += defaultValueStringLength;
            }

            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueFloat, 4);
            bufferPtr += 4;

            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueVector.x, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueVector.y, 4);
            bufferPtr += 4;
            memcpy(bufferPtr, &((ClassDef_Jupiter *)classDefs[i])->m_Props[j].m_DefaultValueVector.z, 4);
            bufferPtr += 4;
        }
    }
    return bufferSize;
}