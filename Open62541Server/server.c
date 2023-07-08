#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "temperature.h"

static UA_StatusCode
getTemperatureMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    double temperature = get_temperature();
    char str[100];
    sprintf(str, "%f", temperature);
    UA_String tmp = UA_STRING_ALLOC(str);
    UA_Variant_setScalarCopy(output, &tmp, &UA_TYPES[UA_TYPES_STRING]);
    UA_String_clear(&tmp);
    return UA_STATUSCODE_GOOD;
}

static void
addGetTemperatureMethod(UA_Server *server) {

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
    outputArgument.name = UA_STRING("MyOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes tempAttr = UA_MethodAttributes_default;
    tempAttr.description = UA_LOCALIZEDTEXT("en-US","Get Temperature");
    tempAttr.displayName = UA_LOCALIZEDTEXT("en-US","Get Temperature");
    tempAttr.executable = true;
    tempAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62540),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(2, "get temperature"),
                            tempAttr, &getTemperatureMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);
}

int main(void) {

    UA_Server *server = UA_Server_new();
    UA_ServerConfig *config = UA_Server_getConfig(server);

    UA_String customServerUrls[1];
    customServerUrls[0] = UA_STRING("opc.tcp://localhost:4845/server");
    size_t customServerUrlsSize = 1;

    config->serverUrls = customServerUrls;
    config->serverUrlsSize = customServerUrlsSize;
    config->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC("en", "Example for Medium");
    UA_Server_addNamespace(server, "http://iot.opcualex.de");
    addGetTemperatureMethod(server);

    UA_StatusCode retval = UA_Server_runUntilInterrupt(server);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
