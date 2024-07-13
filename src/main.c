// windows lean and mean
#define WIN32_LEAN_AND_MEAN

// includes
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/ltdefs.h"

int nDefs = 0;

char *GetPropertyTypeString(short propType)
{
    switch (propType)
    {
        case PT_STRING: return "String";
        case PT_VECTOR: return "Vector";
        case PT_COLOR: return "Color";
        case PT_REAL: return "Real";
        case PT_FLAGS: return "Flags";
        case PT_BOOL: return "Bool";
        case PT_LONGINT: return "LongInt";
        case PT_ROTATION: return "Rotation";
        default: return "Unknown";
    }
}

void PrintClassInformation(eEngineVersion nEngineVersion, void **classDefs)
{
    switch (nEngineVersion)
    {
        case LT1:
        {
            ClassDef_LT1 **classDefsLT1 = (ClassDef_LT1 **)classDefs;
            for (int i = 0; i < nDefs; i++)
            {
                printf("Class Name: %s\n", classDefsLT1[i]->m_ClassName);
                for (int j = 0; j < classDefsLT1[i]->m_nProps; j++)
                {
                    printf("Property Name: %s Type: %s\n", classDefsLT1[i]->m_Props[j].m_PropName, GetPropertyTypeString(classDefsLT1[i]->m_Props[j].m_PropType));
                }
            }
            break;
        }
        case LT2:
        {
            ClassDef_LT2 **classDefsLT2 = (ClassDef_LT2 **)classDefs;
            for (int i = 0; i < nDefs; i++)
            {
                printf("Class Name: %s\n", classDefsLT2[i]->m_ClassName);
                for (int j = 0; j < classDefsLT2[i]->m_nProps; j++)
                {
                    printf("Property Name: %s Type: %s\n", classDefsLT2[i]->m_Props[j].m_PropName, GetPropertyTypeString(classDefsLT2[i]->m_Props[j].m_PropType));
                }
            }
            break;
        }
        case LTTALON:
        {
            ClassDef_Talon **classDefsTalon = (ClassDef_Talon **)classDefs;
            for (int i = 0; i < nDefs; i++)
            {
                printf("Class Name: %s\n", classDefsTalon[i]->m_ClassName);
                for (int j = 0; j < classDefsTalon[i]->m_nProps; j++)
                {
                    printf("Property Name: %s Type: %s\n", classDefsTalon[i]->m_Props[j].m_PropName, GetPropertyTypeString(classDefsTalon[i]->m_Props[j].m_PropType));
                }
            }
            break;
        }
        case LTJUPITER:
        {
            ClassDef_Jupiter **classDefsJupiter = (ClassDef_Jupiter **)classDefs;
            for (int i = 0; i < nDefs; i++)
            {
                printf("Class Name: %s\n", classDefsJupiter[i]->m_ClassName);
                for (int j = 0; j < classDefsJupiter[i]->m_nProps; j++)
                {
                    printf("Property Name: %s Type: %s\n", classDefsJupiter[i]->m_Props[j].m_PropName, GetPropertyTypeString(classDefsJupiter[i]->m_Props[j].m_PropType));
                }
            }
            break;
        }
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <path to object.lto> <engine version: LT1, LT2, TALON, JUPITER>\n", argv[0]);
        return 1;
    }

    eEngineVersion nEngineVersion;
    if (strcmp(argv[2], "LT1") == 0) nEngineVersion = LT1;
    else if (strcmp(argv[2], "LT2") == 0) nEngineVersion = LT2;
    else if (strcmp(argv[2], "TALON") == 0) nEngineVersion = LTTALON;
    else if (strcmp(argv[2], "JUPITER") == 0) nEngineVersion = LTJUPITER;
    else
    {
        printf("Invalid engine version\n");
        return 1;
    }

    HINSTANCE hGetProcIDDLL = LoadLibrary(argv[1]);
    if (!hGetProcIDDLL)
    {
        printf("Could not load the dynamic library\n");
        return 1;
    }

    void **classDefs = NULL;
    int version = 0;

    switch (nEngineVersion)
    {
        case LT1:
        {
            ObjectDLLSetupLT1 ObjectDLLSetupLT1Fn = (ObjectDLLSetupLT1)GetProcAddress(hGetProcIDDLL, "ObjectDLLSetup");
            if (!ObjectDLLSetupLT1Fn)
            {
                printf("Could not locate the function\n");
                FreeLibrary(hGetProcIDDLL);
                return 1;
            }
            classDefs = (void **)ObjectDLLSetupLT1Fn(&nDefs, NULL, &version);
            break;
        }
        case LT2:
        {
            ObjectDLLSetupLT2 ObjectDLLSetupLT2Fn = (ObjectDLLSetupLT2)GetProcAddress(hGetProcIDDLL, "ObjectDLLSetup");
            if (!ObjectDLLSetupLT2Fn)
            {
                printf("Could not locate the function\n");
                FreeLibrary(hGetProcIDDLL);
                return 1;
            }
            classDefs = (void **)ObjectDLLSetupLT2Fn(&nDefs, NULL, &version);
            break;
        }
        case LTTALON:
        {
            ObjectDLLSetupTalon ObjectDLLSetupTalonFn = (ObjectDLLSetupTalon)GetProcAddress(hGetProcIDDLL, "ObjectDLLSetup");
            if (!ObjectDLLSetupTalonFn)
            {
                printf("Could not locate the function\n");
                FreeLibrary(hGetProcIDDLL);
                return 1;
            }
            classDefs = (void **)ObjectDLLSetupTalonFn(&nDefs, NULL, &version);
            break;
        }
        case LTJUPITER:
        {
            ObjectDLLSetupJupiter ObjectDLLSetupJupiterFn = (ObjectDLLSetupJupiter)GetProcAddress(hGetProcIDDLL, "ObjectDLLSetup");
            if (!ObjectDLLSetupJupiterFn)
            {
                printf("Could not locate the function\n");
                FreeLibrary(hGetProcIDDLL);
                return 1;
            }
            classDefs = (void **)ObjectDLLSetupJupiterFn(&nDefs, NULL, &version);
            break;
        }
        default:
            FreeLibrary(hGetProcIDDLL);
            return 1;
    }

    PrintClassInformation(nEngineVersion, classDefs);
    FreeLibrary(hGetProcIDDLL);

    return 0;
}
