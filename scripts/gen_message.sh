#!/usr/bin/env bash

yaml_file=$1
gen_dir=$2
yaml_parser="dasel -n -p yaml --plain -f"
out_header="$gen_dir"/$(basename "$yaml_file").h
out_source="$gen_dir"/$(basename "$yaml_file").cpp

echo "Generating $(basename "$yaml_file") message..."

mkdir -p "$gen_dir"

namespace=$($yaml_parser "$yaml_file" namespace)

{
    printf "%s\n\n" "#pragma once"
    printf "%s\n\n" "#include \"core/src/util/messaging/Messaging.h\""
} > "$out_header"

{
    printf "%s\n\n" "#include \"$(basename $yaml_file).h\""
} > "$out_source"

if [[ $($yaml_parser "$yaml_file" includes) != "null" ]]
then
    declare -i number_of_includes
    number_of_includes=$($yaml_parser "$yaml_file" includes --length)

    for ((i=0; i<number_of_includes; i++))
    do
        include=$($yaml_parser "$yaml_file" includes.["$i"])
        printf "%s\n" "#include \"${include}\"" >> "$out_header"
    done

    printf "\n" >> "$out_header"
fi

if [[ $namespace != "null" ]]
then
    printf "%s\n" "namespace $namespace {" >> "$out_header"
    printf "%s\n\n" "using namespace $namespace;" >> "$out_source"
fi

if [[ $($yaml_parser "$yaml_file" notifications) != "null" ]]
then
    declare -i number_of_notifications
    number_of_notifications=$($yaml_parser "$yaml_file" notifications --length)

    for ((i=0; i<number_of_notifications; i++))
    do
        declare -i number_of_members
        notification_name=$($yaml_parser "$yaml_file" notifications.["$i"].name)
        number_of_data_members=$($yaml_parser "$yaml_file" notifications.["$i"].data --length)
        queue_size=$($yaml_parser "$yaml_file" notifications.["$i"].queue-size)

        {
            printf "%s\n" "class ${notification_name}Notification : public core::util::messaging::Notification {"
            printf "%s\n" "public:"
            printf "%s\n\n" "${notification_name}Notification() = default;"
        } >> "$out_header"

        if [[ $queue_size != "null" ]]
        then
            {
                printf "%s\n" "static constexpr bool USE_QUEUE = true;"
                printf "%s\n\n" "static constexpr size_t QUEUE_SIZE = $queue_size;"
            } >> "$out_header"
        else
            {
                printf "%s\n\n" "static constexpr bool USE_QUEUE = false;"
            } >> "$out_header"
        fi


        if [[ $number_of_data_members != "null" ]]
        then
            printf "%s\n" "struct data_t {" >> "$out_header"

            for ((j=0; j<number_of_data_members; j++))
            do
                type=$($yaml_parser "$yaml_file" notifications.["$i"].data.["$j"].type)
                name=$($yaml_parser "$yaml_file" notifications.["$i"].data.["$j"].name)

                printf "%s\n" "${type} ${name};" >> "$out_header"
            done

            {
                printf "%s\n\n" "};"
                printf "%s\n\n" "data_t data;"
            } >> "$out_header"
        fi
        {
            printf "%s\n" "private:"
            printf "%s\n" "friend class core::util::messaging::Server;"
            printf "%s\n" "static std::weak_ptr<core::util::messaging::Publisher<${notification_name}Notification>> _publisher;"
            printf "%s\n" "static std::vector<std::weak_ptr<core::util::messaging::Subscriber<${notification_name}Notification>>> _subscribers;"
            printf "%s\n\n" "};"
        } >> "$out_header"

        {
            printf "%s\n" "std::vector<std::weak_ptr<core::util::messaging::Subscriber<${notification_name}Notification>>> ${notification_name}Notification::_subscribers;"
            printf "%s\n" "std::weak_ptr<core::util::messaging::Publisher<${notification_name}Notification>> ${notification_name}Notification::_publisher;"
        } >> "$out_source"
    done
fi

printf "\n" >> "$out_header"

if [[ $($yaml_parser "$yaml_file" services) != "null" ]]
then
    declare -i number_of_services
    number_of_services=$($yaml_parser "$yaml_file" services --length)

    for ((i=0; i<number_of_services; i++))
    do
        declare -i number_of_members
        number_of_request_members=$($yaml_parser "$yaml_file" services.["$i"].request --length)
        number_of_response_members=$($yaml_parser "$yaml_file" services.["$i"].response --length)
        service_name=$($yaml_parser "$yaml_file" services.["$i"].name)

        {
            printf "%s\n" "class ${notification_name}Service : public core::util::messaging::Service {"
            printf "%s\n" "public:"
            printf "%s\n\n" "${notification_name}Service() = default;"
        } >> "$out_header"


        if [[ $number_of_request_members != "null" ]]
        then
            printf "%s\n" "struct request_t {" >> "$out_header"

            for ((j=0; j<number_of_request_members; j++))
            do
                type=$($yaml_parser "$yaml_file" services.["$i"].request.["$j"].type)
                name=$($yaml_parser "$yaml_file" services.["$i"].request.["$j"].name)

                printf "%s\n" "${type} ${name};" >> "$out_header"
            done

            {
                printf "%s\n\n" "};"
                printf "%s\n\n" "request_t request;" 
            } >> "$out_header"
        fi

        if [[ $number_of_response_members != "null" ]]
        then
            printf "%s\n" "struct response_t {" >> "$out_header"

            for ((j=0; j<number_of_response_members; j++))
            do
                type=$($yaml_parser "$yaml_file" services.["$i"].response.["$j"].type)
                name=$($yaml_parser "$yaml_file" services.["$i"].response.["$j"].name)

                printf "%s\n" "${type} ${name};" >> "$out_header"
            done

             {
                printf "%s\n\n" "};"
                printf "%s\n\n" "response_t response;" 
            } >> "$out_header"
        fi

        {
            printf "%s\n" "private:"
            printf "%s\n" "friend class core::util::messaging::Server;"
            printf "%s\n" "static std::function<bool(${service_name}Service&)> _serviceCallback;"
            printf "%s\n\n" "};"
        } >> "$out_header"

        {
            printf "%s\n" "std::function<bool(${service_name}Service&)> ${service_name}Service::_serviceCallback;"
        } >> "$out_source"
    done
fi

if [[ $namespace != "null" ]]
then
    printf "%s\n" "}" >> "$out_header"
fi