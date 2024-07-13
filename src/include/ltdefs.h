#pragma once

// Property types
#define PT_STRING 0
#define PT_VECTOR 1
#define PT_COLOR 2
#define PT_REAL 3
#define PT_FLAGS 4
#define PT_BOOL 5
#define PT_LONGINT 6
#define PT_ROTATION 7
#define NUM_PROPERTYTYPES 8

typedef struct DEditInternal DEditInternal;
typedef struct CProjectClass CProjectClass;

// Function pointer typedefs
typedef void (*ConstructObjectFn)(void *pObject);
typedef void (*DestructObjectFn)(void *pObject);
typedef void (*EngineMessageFn)(void *pObject, unsigned long messageID, void *pData, float fData);
typedef void (*ObjectMessageFn)(void *pObject, unsigned long messageID, void *pData, float fData);
typedef struct IObjectPlugin IObjectPlugin;
typedef IObjectPlugin *(*CreateObjectPluginFn)();

typedef struct LTVector {
    float x, y, z;
} LTVector;

// Property definition structures
typedef struct PropDefLT1 {
    char *m_PropName;
    short m_PropType;
    LTVector m_DefaultValueVector;
    float m_DefaultValueFloat;
    char *m_DefaultValueString;
    short m_PropFlags;
    void *m_pInternal;
} PropDefLT1;

typedef struct PropDefLT2 {
    char *m_PropName;
    short m_PropType;
    LTVector m_DefaultValueVector;
    float m_DefaultValueFloat;
    char *m_DefaultValueString;
    unsigned long m_PropFlags;
    DEditInternal *m_pDEditInternal;
    void *m_pInternal;
} PropDefLT2;

typedef struct PropDefTalon {
    char *m_PropName;
    short m_PropType;
    char *m_PropHelp;
    LTVector m_DefaultValueVector;
    float m_DefaultValueFloat;
    char *m_DefaultValueString;
    unsigned long m_PropFlags;
    DEditInternal *m_pDEditInternal;
    void *m_pInternal;
} PropDefTalon;

typedef struct PropDefJupiter {
    char *m_PropName;
    short m_PropType;
    LTVector m_DefaultValueVector;
    float m_DefaultValueFloat;
    char *m_DefaultValueString;
    unsigned long m_PropFlags;
    DEditInternal *m_pDEditInternal;
    void *m_pInternal;
} PropDefJupiter;

// Class definition structures
typedef struct ClassDef_LT1 {
    char *m_ClassName;
    struct ClassDef_LT1 *m_ParentClass;
    unsigned int m_ClassFlags;
    ConstructObjectFn m_ConstructFn;
    DestructObjectFn m_DestructFn;
    EngineMessageFn m_EngineMessageFn;
    ObjectMessageFn m_ObjectMessageFn;
    short m_nProps;
    PropDefLT1 *m_Props;
    long m_ClassObjectSize;
    void *m_pInternal[2];
} ClassDef_LT1;

typedef struct ClassDef_LT2 {
    char *m_ClassName;
    struct ClassDef_LT2 *m_ParentClass;
    unsigned int m_ClassFlags;
    ConstructObjectFn m_ConstructFn;
    DestructObjectFn m_DestructFn;
    CreateObjectPluginFn m_PluginFn;
    short m_nProps;
    PropDefLT2 *m_Props;
    long m_ClassObjectSize;
    void *m_pInternal[2];
} ClassDef_LT2;

typedef struct ClassDef_Talon {
    char *m_ClassName;
    struct ClassDef_Talon *m_ParentClass;
    unsigned int m_ClassFlags;
    ConstructObjectFn m_ConstructFn;
    DestructObjectFn m_DestructFn;
    CreateObjectPluginFn m_PluginFn;
    short m_nProps;
    PropDefTalon *m_Props;
    long m_ClassObjectSize;
    void *m_pInternal[2];
} ClassDef_Talon;

typedef struct ClassDef_Jupiter {
    char *m_ClassName;
    struct ClassDef_Jupiter *m_ParentClass;
    unsigned int m_ClassFlags;
    ConstructObjectFn m_ConstructFn;
    DestructObjectFn m_DestructFn;
    CreateObjectPluginFn m_PluginFn;
    short m_nProps;
    PropDefJupiter *m_Props;
    long m_ClassObjectSize;
    void *m_pInternal[2];
} ClassDef_Jupiter;

// Function pointer typedefs for ObjectDLLSetup for each engine version
typedef ClassDef_LT1 **(*ObjectDLLSetupLT1)(int *, void *, int *);
typedef ClassDef_LT2 **(*ObjectDLLSetupLT2)(int *, void *, int *);
typedef ClassDef_Talon **(*ObjectDLLSetupTalon)(int *, void *, int *);
typedef ClassDef_Jupiter **(*ObjectDLLSetupJupiter)(int *, void *, int *);

typedef enum {
    NONE = 0,
    LT1 = 1,
    LT2 = 2,
    LTTALON = 3,
    LTJUPITER = 4
} eEngineVersion;
