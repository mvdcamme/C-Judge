import json
import sys

def generate_message_json(description):
	return {"format": "code", "description": description, "permission": "student"}
	
def generate_compiler_error_feedback_json(message_jsons):
	return {"accepted": False, "status": "compilation error", "messages": message_jsons}
			
def write_feedback_json(feedback_json, path_to_output):
	with open(path_to_output, mode = 'w') as feedback_file:
		feedback_json_string = json.dumps(feedback_json, indent = 4)
		feedback_file.write(feedback_json_string)
		
def read_error_file(path_to_error_file):
	with open(path_to_error_file, mode = 'r') as error_file:
		file_content = error_file.read()
		lines = file_content.splitlines()
		all_messages = list(map(generate_message_json, lines))
		feedback_json = generate_compiler_error_feedback_json(all_messages)
		return feedback_json
		
feedback_json = read_error_file(sys.argv[1])
write_feedback_json(feedback_json, sys.argv[2])
