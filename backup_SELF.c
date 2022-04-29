// if(recv_key > node->self_key)
        // {
        //     //Node is alone in ring
        //     if(node->succ_key == -1 && node->pred_key == node->self_key)
        //     {
        //         printf("Alone in ring!\n");
        //         sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

        //         node->pred_key = node->succ_key;
        //         strcpy(node->pred_IP, node->succ_IP);
        //         strcpy(node->pred_Port, node->succ_Port);

        //         printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);

        //         msg_create(retmsg, "SELF" , node);

        //         printf("MESSAGE TO SUCC %d:\t%s\n", node->pred_key, retmsg);

        //         node->fd_succ = node->fd_aux;
        //         node->fd_aux = -1;

        //         write_tcp(&node->fd_succ, retmsg);
                
        //     }
        //     else if(recv_key != node->succ_key && recv_key != node->pred_key)
        //     {
        //         //Normal entering mode
        //         if(recv_key > node->self_key && node->succ_key != -1)
        //         {
        //             printf("Normal!\n");

        //             short i = node->succ_key;

        //             //It has to inform its old successor of
        //             //its new predecessor (the old successor's)

        //             sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);

        //             //Creates a PRED message to send to it
        //             msg_create(retmsg, "PRED" , node);

        //             printf("MESSAGE TO OLD SUCCESSOR %d:\t%s\n", i, retmsg);

        //             write_tcp(&node->fd_succ, retmsg);

        //             //It can close its connection to the previous
        //             //succ, since it is no more
        //             close_tcp(&node->fd_succ);

        //             node->fd_succ = node->fd_aux;
        //             node->fd_aux = -1;

        //             //Updates succ info with newly entered node
        //             printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);

                    
        //         }
        //         //Arriving node can't connect with self as
        //         //its pred
        //         else if(recv_key > node->succ_key && node->succ_key != -1)
        //         {
        //             write_tcp(&node->fd_aux, "CAN'T CONNECT HERE!\n");
        //             close_tcp(&node->fd_aux);

        //         }
        //         else if(recv_key > node->self_key)//Its the succ identifying itself
        //         {
        //             printf("GOT UR INFO SUCC\n");
        //             sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
        //             node->fd_succ = node->fd_aux;
        //             node->fd_aux = -1;

        //             printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);
        //         }
        //         else if(recv_key != node->succ_key)//Keys are the same and it's not the succ node messaging
        //         {
        //             write_tcp(&node->fd_aux, "There's already a key equal to yours! [DISCONNECTING]\n");
        //             close_tcp(&node->fd_aux);
        //         }
        //     }//Not alone
        //     /* else if(recv_key != node->succ_key)
        //     {
        //         printf("GOT UR INFO SUCC\n");
        //         sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
        //         node->fd_succ = node->fd_aux;
        //         node->fd_aux = 0;

        //         printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);
        //     } */
        // } // > self
        // else
        // {
        //     printf("GOT UR INFO SUCC2\n");
        //     sscanf(msg, "%*s %hd %s %s", &node->succ_key, node->succ_IP, node->succ_Port);
            
        //     printf("NEW SUCC:\nKEY:\t%hd\nIP:\t%s\nPORT:\t%s\n", node->succ_key, node->succ_IP, node->succ_Port);

        //     node->fd_aux = -1;
        // }