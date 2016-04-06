#include web_server.c
#include web_server.h
#include gainspan_gs1011m.h
#include gainspan_gs1011m.c
#include web_page.h

#ifndef web_page
#define web_page

/*! \brief Creates the web server and web page with radio button input to control Chico
 */
void createWebPage() {

	SET_WEB_SERVER_TERMINAL_OUTPUT_ON = 1; 
	configure_web_page("Chico: The Robot", " Control Interface ", HTML_RADIO_BUTTON);
	add_element_choice('0', "FORWARDS");
	add_element_choice ('1', "BACKWARDS");
	add_element_choice ('2', "CLOCKWISE");
	add_element_choice ('3', "COUNTERCLOCKWISE");
	start_web_server();
}

/*! \brief Receives user input and returns the user-entered value
 *  Returns a char which which will be used to determine which direction to move Chico in.
 */
char getClientResponse ()
{
	process_client_request();
	return get_next_client_response();
}

#endif