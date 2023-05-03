#include "Server.hpp"

// **********************************************************************************************
// Add or retrieve the topic associated to a channel (mandatory)
void Server::Cmds_topic(int const fd_client, std::string const command)
{
    std::cout << RED << fd_client << "|" << command << "|" << NOC << std::endl;
    std::string topic = "";

	if (command.find("TOPIC") == 0)
		topic = command.substr(5);
	else
		topic = command;

    // the entry message look's like : TOPIC #blabla :test ajout de topic


    // detect if the topic set is requested by the command
    if (topic.find(" :") < topic.length())
    {
            // extract the channel
        std::string channel = topic.substr(topic.find("#")+1, topic.find(" :")-2);
        std::cout << RED << "channel |" << channel << "|" << channel.length() << NOC << std::endl;

        // extract the topic only
        topic = topic.substr(topic.find(" :")+2);

        // cut the \r\n at the end of the string
        topic = topic.substr(0, topic.length()-2);        

        std::cout << RED << "Topic |" << topic << "|" << topic.length() << NOC << std::endl;
        
        // ensure the requester is the channel's owner
        std::string mode = this->_channels[channel]->getChannelConnectedFDMode(fd_client);
        std::cout << RED << "Mode : " << mode << NOC << std::endl;

        // Find Nickname
        std::string nickname = this->_fd_nick_list[fd_client];
        // Find user
        std::string user_client = this->_clientList[nickname]->get_user();
        // Find IP address
        std::string ip_client = this->_clientList[nickname]->get_ip();

        std::string hostname = this->_hostname;

        if (mode == "O@")
        {
            // place the topic value into the channel
             this->_channels[channel]->setChannelTopic(topic);



            // ------------------
			// send message about new topic e.g. :
            // :VRO!~VRoch@185.25.195.181 TOPIC #blabla1 :ceci est un channel de test
			std::string cap_response = ":" + nickname + "!~" + user_client + '@' + ip_client + " TOPIC " + "#" + channel + " :" + topic + "\r\n";

			Cmds_inform_Channel(cap_response.c_str(), channel, "");
        }
        else
        {
            std::cout << RED << "ELSE : " << mode << NOC << std::endl;
            // inform the requester her can't change the topic
            // :kinetic.oftc.net 482 VRO_D1 #blabla1 :You're not channel operator
            std::string cap_response = ":" + hostname + " 482 " + nickname + " #" + channel + " :You're not channel operator\r\n";
            std::cout << fd_client << " [Server->Client]" << cap_response << std::endl;

            send(fd_client, cap_response.c_str(), cap_response.length(), 0);
        }

    }

    



}