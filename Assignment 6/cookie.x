struct arguments {
    int responseVal;
    int client_id;
};

program SLW_COOKIES {
    version ASSIGNMENT_6 {
        int GET_ME_MY_COOKIE(struct arguments) = 1;
    } = 1;
} = 96;