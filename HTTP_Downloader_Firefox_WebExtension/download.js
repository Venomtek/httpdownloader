var g_initial_height = 0;
var g_theme_support;

function CloseWindow( window_id )
{
	if ( window_id == null )
	{
		window_id = parseInt( document.getElementById( "window_id" ).value );
	}

	browser.runtime.sendMessage(
	{
		type: "close_window",
		window_id: window_id
	} )
	.then( function( info )
	{
		browser.windows.remove( window_id );
	} );
}

function SendDownloadToClient( add_type )
{
	browser.runtime.sendMessage(
	{
		type: "server_info"
	} )
	.then( function( info )
	{
		var request = new XMLHttpRequest();
		if ( request )
		{
			var window_id = parseInt( document.getElementById( "window_id" ).value );

			var server = info.server;

			var server_username = atob( info.username );
			var server_password = atob( info.password );

			var urls = document.getElementById( "urls" ).value;
			if ( urls != "" )
			{
				urls = urls.trim();
				urls = urls.replace( /\r?\n/g, "\r\n" );
				urls = urls.replace( /(\r\n)+/g, "\r\n" );
				urls += "\r\n";
			}

			var username = document.getElementById( "username" ).value;
			var password = document.getElementById( "password" ).value;

			var enable_directory = document.getElementById( "enable_directory" ).checked;
			var directory = "";
			if ( enable_directory )
			{
				directory = document.getElementById( "directory" ).value;
			}

			var enable_parts = document.getElementById( "enable_parts" ).checked;
			var parts = "";
			if ( enable_parts )
			{
				parts = document.getElementById( "parts" ).value;
			}

			var ssl_tls_version = document.getElementById( "ssl_tls_version" ).value;

			var enable_speed_limit = document.getElementById( "enable_speed_limit" ).checked;
			var speed_limit = "";
			if ( enable_speed_limit )
			{
				speed_limit = document.getElementById( "speed_limit" ).value;
			}

			var download_operations = add_type | ( info.override_prompts ? 2 : 0 ) | ( document.getElementById( "simulate_download" ).checked ? 1 : 0 );

			var cookies = document.getElementById( "cookies" ).value;

			var headers = document.getElementById( "headers" ).value;
			if ( headers != "" )
			{
				headers = headers.trim();
				headers = headers.replace( /\r?\n/g, "\r\n" );
				headers = headers.replace( /(\r\n)+/g, "\r\n" );
				headers += "\r\n";
			}

			var method = "1";	// GET
			var post_data = "";
			if ( document.getElementById( "method" ).checked )
			{
				method = "2";	// POST
				post_data = document.getElementById( "post_data" ).value;
			}

			var proxy_hostname_ip = "";
			var proxy_port = "";
			var proxy_use_authentication = "";
			var proxy_username = "";
			var proxy_password = "";
			var proxy_resolve_domain_names = "";

			var proxy_type = document.getElementById( "proxy_type" ).value;

			if ( proxy_type != 0 )
			{
				proxy_hostname_ip = document.getElementById( "proxy_hostname_ip" ).value;
				proxy_port = document.getElementById( "proxy_port" ).value;

				if ( proxy_type == 1 || proxy_type == 2 )
				{
					proxy_username = document.getElementById( "proxy_username" ).value;
					proxy_password = document.getElementById( "proxy_password" ).value;
				}
				else if ( proxy_type == 3 )
				{
					proxy_username = document.getElementById( "proxy_ident_username" ).value;

					proxy_resolve_domain_names = document.getElementById( "proxy_resolve_domain_names_v4a" ).checked;
				}
				else if ( proxy_type == 4 )
				{
					proxy_use_authentication = document.getElementById( "proxy_use_authentication" ).checked;

					if ( proxy_use_authentication )
					{
						proxy_username = document.getElementById( "proxy_socks5_username" ).value;
						proxy_password = document.getElementById( "proxy_socks5_password" ).value;
					}

					proxy_resolve_domain_names = document.getElementById( "proxy_resolve_domain_names" ).checked;
				}
			}

			var download = document.getElementById( "download" );
			download.disabled = true;

			var add = document.getElementById( "add" );
			add.disabled = true;

			request.onerror = function( e )
			{
				download.disabled = false;
				add.disabled = false;

				window.alert( browser.i18n.getMessage( "SEND_FAILED" ) );
			};

			request.ontimeout = function( e )
			{
				download.disabled = false;
				add.disabled = false;

				window.alert( browser.i18n.getMessage( "CONNECTION_TIMEOUT" ) );
			};

			request.onload = function( e )
			{
				if ( request.responseText == "DOWNLOADING" )
				{
					CloseWindow( window_id );
				}
				else
				{
					download.disabled = false;
					add.disabled = false;

					window.alert( browser.i18n.getMessage( "INVALID_RESPONSE" ) );
				}
			};

			if ( server_username != "" || server_password != "" )
			{
				request.open( "POST", server, true, server_username, server_password );
				request.withCredentials = true;
			}
			else
			{
				request.open( "POST", server, true );
			}
			request.timeout = 30000;	// 30 second timeout.
			request.setRequestHeader( "Content-Type", "application/octet-stream" );
			request.send( method + "\x1f" +
						  urls + "\x1f" +
						  directory + "\x1f" +
						  parts + "\x1f" +
						  ssl_tls_version + "\x1f" +
						  username + "\x1f" +
						  password + "\x1f" +
						  speed_limit + "\x1f" +
						  download_operations + "\x1f" +
						  cookies + "\x1f" +
						  headers + "\x1f" +
						  post_data + "\x1f" +
						  proxy_type + "\x1f" +
						  proxy_hostname_ip + "\x1f" +
						  proxy_port + "\x1f" +
						  proxy_username + "\x1f" +
						  proxy_password + "\x1f" +
						  proxy_resolve_domain_names + "\x1f" +
						  proxy_use_authentication + "\x1f" );
		}
		else
		{
			console.log( "Failed to create XMLHttpRequest." );
		}
	} );
}

function SendPOSTData()
{
	document.getElementById( "post_data" ).disabled = !event.target.checked;
}

function GetDownloadInfo( id )
{
	browser.runtime.sendMessage(
	{
		type: "loading",
		window_id: id
	} )
	.then( function( info )
	{
		g_theme_support = info.theme_support;

		browser.theme.getCurrent().then( function ( theme )
		{
			SetThemeStyle( theme );
		} );

		var urls;

		if ( info.filename != "" )
		{
			urls = "[" + info.filename + "]" + info.urls;
		}
		else
		{
			urls = info.urls;
		}

		document.getElementById( "window_id" ).value = id;
		document.getElementById( "last_urls" ).value = urls;
		document.getElementById( "urls" ).value = urls;
		document.getElementById( "cookies" ).value = ( info.cookies ? info.cookies : "" );
		document.getElementById( "headers" ).value = ( info.headers ? info.headers : "" );
		var enable_directory = document.getElementById( "enable_directory" );
		var directory = document.getElementById( "directory" );
		if ( info.directory )
		{
			enable_directory.checked = true;
			directory.value = info.directory;
			directory.disabled = false;
		}
		else
		{
			enable_directory.checked = false;
			directory.value = "";
			directory.disabled = true;
		}
		var enable_post = ( info.method == 2 ? true : false );
		document.getElementById( "method" ).checked = enable_post;
		document.getElementById( "post_data" ).value = ( info.post_data ? info.post_data : "" );
		document.getElementById( "post_data" ).disabled = !enable_post;

		if ( info.message && info.message != "" )
		{
			window.alert( info.message );
		}
	} );
}

function SetFliterPreset()
{
	var filter_preset = document.getElementById( "filter_presets" ).value;

	if ( filter_preset == "Images" )
	{
		document.getElementById( "filter" ).value = "^((http|ftpe?)s?|sftp):\\/\\/[^\\/\\s]+\\/[^\\?#\\s]+\\.(jp(e?g|e)|gif|png|bmp|tiff?|dib|ico)(\\?|#|$)";
	}
	else if ( filter_preset == "Music" )
	{
		document.getElementById( "filter" ).value = "^((http|ftpe?)s?|sftp):\\/\\/[^\\/\\s]+\\/[^\\?#\\s]+\\.(mp3|wave?|flac?|ogg|m4a|wma|aac|midi?|ape|shn|wv|aiff?|oga)(\\?|#|$)";
	}
	else if ( filter_preset == "Videos" )
	{
		document.getElementById( "filter" ).value = "^((http|ftpe?)s?|sftp):\\/\\/[^\\/\\s]+\\/[^\\?#\\s]+\\.(avi|mp[124]|m4v|mp(e?g|e)|mkv|webm|wmv|3gp|ogm|ogv|flv|vob)(\\?|#|$)";
	}
	else
	{
		document.getElementById( "filter" ).value = "";
	}
}

function ApplyFliter()
{
	var filter = new RegExp( document.getElementById( "filter" ).value, "i" );

	var urls = document.getElementById( "urls" ).value;
	urls = urls.replace( /\r?\n/g, "\r\n" );
	if ( !urls.endsWith( "\r\n" ) )
	{
		urls += "\r\n";
	}

	document.getElementById( "last_urls" ).value = document.getElementById( "urls" ).value;

	var url_array = urls.split( "\r\n" );
	urls = "";

	var url_count = url_array.length;
	if ( url_count > 0 )
	{
		--url_count;
	}

	for ( var i = 0; i < url_count; ++i )
	{
		if ( filter.test( url_array[ i ] ) )
		{
			urls += url_array[ i ] + "\r\n";
		}
	}

	document.getElementById( "urls" ).value = urls;
}

function RevertFliter()
{
	document.getElementById( "urls" ).value = document.getElementById( "last_urls" ).value;
}

function EnableDirectory( event )
{
	document.getElementById( "directory" ).disabled = !event.target.checked;
}

function EnableParts( event )
{
	document.getElementById( "parts" ).disabled = !event.target.checked;
}

function EnableSpeedLimit( event )
{
	document.getElementById( "speed_limit" ).disabled = !event.target.checked;
}

function SimulateDownload( event )
{
	var cb = document.getElementById( "enable_directory" );

	cb.disabled = event.target.checked;
	cb.nextSibling.style.color = ( event.target.checked ? "grey" : "" );

	if ( document.getElementById( "enable_directory" ).checked )
	{
		document.getElementById( "directory" ).disabled = event.target.checked;
	}
}

function ShowHideProxyTabs( event )
{
	var p = document.querySelectorAll( "#proxy_tab > div.proxy_tab_panel" );

	for ( var i = 1; i < p.length; ++i )
	{
		p[ i ].style.display = "none";
	}

	if ( event.target.value == 0 )
	{
		p[ 0 ].style.display = "none";
	}
	else
	{
		var proxy_port = document.getElementById( "proxy_port" );

		if ( event.target.value == 1 ) { proxy_port.value = "80"; }
		else if ( event.target.value == 2 ) { proxy_port.value = "443"; }
		else if ( event.target.value == 3 ) { proxy_port.value = "1080"; }
		else if ( event.target.value == 4 ) { proxy_port.value = "1080"; }

		p[ 0 ].style.display = "block";

		var index = ( event.target.value > 2 ? event.target.value - 1 : 1 );

		p[ index ].style.display = "block";
	}
}

function UseAuthentication( event )
{
	var u = document.getElementById( "proxy_socks5_username_label" );
	var p = document.getElementById( "proxy_socks5_password_label" );

	if ( event.target.checked )
	{
		u.style.color = "";
		p.style.color = "";
	}
	else
	{
		u.style.color = "grey";
		p.style.color = "grey";
	}

	document.getElementById( "proxy_socks5_username" ).disabled = !event.target.checked;
	document.getElementById( "proxy_socks5_password" ).disabled = !event.target.checked;
}

function AdvancedOptions( event )
{
	browser.windows.getCurrent().then( function( window_info )
	{
		var advanced_info = document.getElementById( "advanced_info" );

		if ( event.target.checked )
		{
			advanced_info.style.display = "block";	// Show.

			// The URL and Directory fields + Advanced fields + 15 px top margin for the Advanced fields.
			document.body.style.minHeight = ( g_initial_height + advanced_info.clientHeight + 15 ) + "px";

			if ( window_info.state != "maximized" )
			{
				// The height of the toolbars and borders + the height of all fields.
				var height = ( window_info.height + advanced_info.clientHeight ) + 15;

				browser.windows.update( window_info.id,
				{
					height: height
				} );
			}
		}
		else
		{
			// Need to save the height before we hide the Advanced fields.
			var advanced_info_height = advanced_info.clientHeight;

			advanced_info.style.display = "none";	// Hide.
			
			document.body.style.minHeight = g_initial_height + "px";

			if ( window_info.state != "maximized" )
			{
				// Exclude the Advanced fields + 15 px top margin from the current window height.
				var height = ( window_info.height - advanced_info_height ) - 15;

				browser.windows.update( window_info.id,
				{
					height: height
				} );
			}
		}
	} );
}

function SetThemeStyle( theme )
{
	if ( g_theme_support && theme.colors )
	{
		var style = document.getElementById( "theme" );
		if ( style == null )
		{
			style = document.createElement( "style" );
			style.id = "theme";
		}
		else if ( style.firstChild != null )
		{
			style.removeChild( style.firstChild );
		}

		var style_text = "";

		var body = "";
		if ( theme.colors.frame ) { body += "background-color: " + theme.colors.frame + ";"; }
		if ( theme.colors.tab_text ) { body += "color: " + theme.colors.tab_text + ";"; }

		var tab_frame = "";
		if ( theme.colors.frame ) { tab_frame += "background-color: " + theme.colors.frame + ";"; }
		if ( theme.colors.input_border ) { tab_frame += "border-color: " + theme.colors.input_border + ";"; }

		var input_label = "";
		if ( theme.colors.button ) { input_label += "background-color: " + theme.colors.button + ";"; }
		else if ( theme.colors.button_background_hover ) { input_label += "background-color: " + theme.colors.button_background_hover + ";"; }
		else if ( theme.colors.button_hover ) { input_label_hover += "background-color: " + theme.colors.button_hover + ";"; }
		if ( theme.colors.input_border ) { input_label += "border-color: " + theme.colors.input_border + ";"; }

		var input_label_hover = "";
		if ( theme.colors.button_background_hover ) { input_label_hover += "background-color: " + theme.colors.button_background_hover + ";"; }
		else if ( theme.colors.button_hover ) { input_label_hover += "background-color: " + theme.colors.button_hover + ";"; }

		var input_checked = "";
		if ( theme.colors.frame ) { input_checked += "background-color: " + theme.colors.frame + ";"; }

		var inputs = "";
		if ( theme.colors.input_background ) { inputs += "background-color: " + theme.colors.input_background + ";"; }
		else if ( theme.colors.toolbar_field ) { inputs += "background-color: " + theme.colors.toolbar_field + ";"; }
		if ( theme.colors.input_border ) { inputs += "border-color: " + theme.colors.input_border + ";"; }
		//else if ( theme.colors.toolbar_field_border ) { inputs += "border-color: " + theme.colors.toolbar_field_border + ";"; }
		if ( theme.colors.input_color ) { inputs += "color: " + theme.colors.input_color + ";"; }
		else if ( theme.colors.toolbar_field_text ) { inputs += "color: " + theme.colors.toolbar_field_text + ";"; }

		var fieldset = "";
		if ( theme.colors.input_border ) { fieldset += "border-color: " + theme.colors.input_border + ";"; }
		//else if ( theme.colors.toolbar_field_border ) { fieldset += "border-color: " + theme.colors.toolbar_field_border + ";"; }

		if ( body != "" ) { style_text += "body {" + body + "}"; }
		if ( tab_frame != "" ) { style_text += ".tab_frame {" + tab_frame + "}"; }
		if ( input_label != "" ) { style_text += ".tab > input + label {" + input_label + "}"; }
		if ( input_label_hover != "" ) { style_text += ".tab > input + label:hover {" + input_label_hover + "}"; }
		if ( input_checked != "" ) { style_text += ".tab > input:checked + label {" + input_checked + "}"; }
		if ( inputs != "" ) { style_text += "input, textarea, select {" + inputs + "}"; }
		if ( fieldset != "" ) { style_text += "fieldset {" + fieldset + "}"; }

		if ( style_text != "" )
		{
			style.appendChild( document.createTextNode( style_text ) );
			document.getElementsByTagName( "head" )[ 0 ].appendChild( style );
		}
	}
	else
	{
		var style = document.getElementById( "theme" );
		if ( style != null )
		{
			style.remove();
		}
	}
}

function UpdateTheme( show )
{
	if ( show )
	{
		browser.theme.getCurrent().then( function ( theme )
		{
			SetThemeStyle( theme );
		} );
	}
	else
	{
		var style = document.getElementById( "theme" );
		if ( style != null )
		{
			style.remove();
		}
	}
}

function UpdateThemeStyle( info )
{
	SetThemeStyle( info.theme );
}

function HandleMessages( request, sender, sendResponse )
{
	if ( request.type == "theme_support" )
	{
		g_theme_support = request.value;

		UpdateTheme( g_theme_support );

		sendResponse( {} );
	}

	return true;
}

document.addEventListener( "contextmenu", function( e )
{
	if ( e.target.nodeName !== "TEXTAREA" && e.target.nodeName !== "INPUT" || ( e.target.nodeName === "INPUT" && ( e.target.type !== "text" && e.target.type !== "number" && e.target.type !== "password" ) ) )
	{
		e.preventDefault();
	}
}, false );

document.addEventListener( "DOMContentLoaded", function()
{
	browser.windows.getCurrent().then( function( window_info )
	{
		GetDownloadInfo( window_info.id );
	} );

	document.querySelectorAll( "[data-i18n]" ).forEach( el =>
	{
		if ( el.id == "directory" )
		{
			el.placeholder = browser.i18n.getMessage( el.dataset.i18n );
		}
		else
		{
			el.innerText = browser.i18n.getMessage( el.dataset.i18n );
		}
	} );

	document.getElementById( "filter_presets" ).addEventListener( "change", SetFliterPreset );
	document.getElementById( "apply_filter" ).addEventListener( "click", ApplyFliter );
	document.getElementById( "revert_filter" ).addEventListener( "click", RevertFliter );
	document.getElementById( "enable_directory" ).addEventListener( "change", EnableDirectory );
	document.getElementById( "enable_parts" ).addEventListener( "change", EnableParts );
	document.getElementById( "enable_speed_limit" ).addEventListener( "change", EnableSpeedLimit );
	document.getElementById( "simulate_download" ).addEventListener( "change", SimulateDownload );
	document.getElementById( "proxy_type" ).addEventListener( "change", ShowHideProxyTabs );
	document.getElementById( "proxy_use_authentication" ).addEventListener( "change", UseAuthentication );
	document.getElementById( "method" ).addEventListener( "change", SendPOSTData );
	document.getElementById( "advanced" ).addEventListener( "click", AdvancedOptions );
	document.getElementById( "download" ).addEventListener( "click", () => { SendDownloadToClient( 0 ); } );
	document.getElementById( "add" ).addEventListener( "click", () => { SendDownloadToClient( 4 ); } );
	document.getElementById( "verify" ).addEventListener( "click", () => { SendDownloadToClient( 1024 ); } );
	document.getElementById( "cancel" ).addEventListener( "click", () => { CloseWindow( null ) } );

	browser.theme.onUpdated.addListener( UpdateThemeStyle );
	browser.runtime.onMessage.addListener( HandleMessages );

	g_initial_height = document.body.clientHeight;
	document.body.style.minHeight = g_initial_height + "px";
} );
