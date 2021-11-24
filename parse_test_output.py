#!/usr/bin/python

import json
import os
import sys

class NoTestOutputFilesSpecified:
	def to_feedback_json(self):
		feedback_json = {"accepted": False, "status": "runtime error", "description": "Tests did not generate any output"}
		return feedback_json
	def __init__(self):
		pass

class Feedback:
	def __init__(self, test_suites = []):
		pass
		
class UnexpectedFailureContext:
	def number_of_failures(self):
		return 1
	def is_failed(self):
		return True
	def to_json(self):
		test_json = {"accepted": False, "generated": "", "expected": "", "messages": []}
		description_json = {"format": "code", "description": self.description, "permission": "student"}
		testcase_json = {"accepted": False, "description": description_json, "tests": [test_json], "messages": []}
		return {"accepted": False, "groups": [testcase_json]}
	def __init__(self, description):
		self.description = "An unexpected failure occurred while executing \"" + description + "\""
		
class Context:
	"""{
	  "name": "Positive",
	  "status": "RUN",
	  "result": "COMPLETED",
	  "timestamp": "2020-07-11T14:49:22Z",
	  "time": "0s",
	  "classname": "FactorialTest",
	  "description": "fac(-5)"
	  "failures": [
		{
		  "failure": "..\/wpo1_ex1\/ex1_unittest_7_.cpp:8\nExpected equality of these values:\n  6\n  fac(3)\n    Which is: 42",
		  "type": ""
		}
	  ]
	}"""
	def number_of_failures(self):
		return len(self.failures)
	def is_failed(self):
		return self.number_of_failures() > 0
	def to_json(self):
		accepted = not self.is_failed()
		test_json = {"accepted": accepted, "generated": self.actual, "expected": self.expected, "messages": []}
		description_json = {"format": "code", "description": self.description, "permission": "student"}
		testcase_json = {"accepted": accepted, "description": description_json, "tests": [test_json], "messages": []}
		return {"accepted": accepted, "groups": [testcase_json]}
	def __init__(self, json_context):
		self.name = json_context["name"] # Not used at the moment
		self.description = json_context["description"]
		self.expected = json_context["expected"]
		self.actual = json_context["actual"]
		try:
			self.failures = json_context["failures"]
		except KeyError:
			self.failures = []
		
class SingleTab:
	def number_of_failures(self):
		return self.context.number_of_failures()
	def is_failed(self):
		return self.context.is_failed
	def to_json(self):
		groups = [self.context.to_json()]
		return {"description": self.name, "groups": groups}
	def __init__(self, name, context):
		self.name = name
		self.context = context
		
def get_test_case_descriptor(json_context):
	if ("description" in json_context):
		return json_context["description"]
	elif ("name" in json_context):
		return json_context["name"]
	else:
		return "unknown_test"
		
class Tab:
	"""{
	"name": "FactorialTest",
	"tests": 1,
	"failures": 1,
	"disabled": 0,
	"errors": 0,
	"timestamp": "2020-07-11T14:49:22Z",
	"time": "0s",
	"testsuite": [ Contexts
	"""
	def add_prepared_context(self, context):
		self.contexts.append(context)
	def add_context(self, json_context):
		try:
			context = Context(json_context)
			self.add_prepared_context(context)
		except KeyError:
			description = get_test_case_descriptor(json_context)
			context = UnexpectedFailureContext(description)
			self.add_prepared_context(context)
	def add_contexts(self, json_contexts):
		for json_context in json_contexts:
			self.add_context(json_context)
	def number_of_failures(self):
		return sum(map(lambda context: context.number_of_failures(), self.contexts))
	def is_failed(self):
		return self.number_of_failures() > 0
	def to_json(self):
		groups = []
		for context in self.contexts:
			groups.append(context.to_json())
		return {"description": self.name, "groups": groups}
	def __init__(self, json_tab):
		self.name = json_tab["name"]
		self.contexts = []
		self.add_contexts(json_tab["testsuite"])
		
		
class AggregatedOutput:
	"""{
	"tests": 1,
	"failures": 1,
	"disabled": 0,
	"errors": 0,
	"timestamp": "2020-07-11T14:49:22Z",
	"time": "0s",
	"name": "AllTests",
	"testsuites": [ Tabs
	"""
	def add_tab(self, json_tab):
		new_tab_name = json_tab["name"]
		for tab in self.tabs:
			if tab.name == new_tab_name:
				tab.add_contexts(json_tab["testsuite"])
				return
		tab = Tab(json_tab)
		self.tabs.append(tab)
	def add_tabs(self, json_tabs):
		for json_tab in json_tabs:
			self.add_tab(json_tab)
	def add_test_failure(self, failure_path):
		try:
			with open(failure_path, mode = "r") as failure_file:
				tab_name = failure_file.readline()[:-1]
				context_name = failure_file.readline()[:-1] # Not actually used at the moment
				description = failure_file.readline()[:-1]
				unexpected_failure_context = UnexpectedFailureContext(description)
				single_tab = SingleTab(tab_name, unexpected_failure_context)
				for tab in self.tabs:
					if tab.name == tab_name:
						tab.add_prepared_context(unexpected_failure_context)
						return
				self.tabs.append(single_tab)
		except IOError:
			pass
	def number_of_failures(self):
		return sum(map(lambda tab: tab.number_of_failures(), self.tabs))
	def is_failed(self):
		return self.number_of_failures() > 0
	def to_feedback_json(self):
		def tabs_to_groups():
			groups = []
			for tab in self.tabs:
				groups.append(tab.to_json())
			return groups
		accepted = not self.is_failed()
		status = "correct" if accepted else "wrong"
		
		feedback_json = {"accepted": accepted, "status": status, "groups": tabs_to_groups()}
		if not accepted:
			feedback_json["description"] = str(self.number_of_failures()) + " failures encountered"
		return feedback_json
	def __init__(self):
		self.tabs = []
	
	
def parse_json_file(path_to_json_file):
	def make_new_json():
		return {'testsuites': []}
	try:
		with open(path_to_json_file, mode = 'r') as json_file:
			json_file_content = json_file.read()
			# https://docs.python.org/3/library/json.html
			acc_json = json.loads(json_file_content)
			return acc_json
	except IOError as e:
		acc_json = make_new_json()
		return acc_json
		
def write_aggregated_output(aggregated_output, path_to_output):
	with open(path_to_output, mode = 'w') as file_aggregated_output:
		feedback_json = aggregated_output.to_feedback_json()
		feedback_json_string = json.dumps(feedback_json, indent = 4)
		file_aggregated_output.write(feedback_json_string)
		
def is_test_output(file_name):
	return file_name.startswith("output_googletest_") and file_name.endswith(".json")
def is_unexpected_output(file_name):
	return file_name.startswith("unexpected_output_") and file_name.endswith(".txt")
							
aggregated_output = AggregatedOutput()
path_to_output = sys.argv[1]

if len(sys.argv) >= 3 and sys.argv[2] == "output_googletest_*.json":
	# No test_output files passed to this script
	write_aggregated_output(NoTestOutputFilesSpecified(), path_to_output)
else:
	for path in sys.argv[2:]:
		if is_test_output(path):
			json_test_output = parse_json_file(path)
			json_tabs = json_test_output["testsuites"]
			aggregated_output.add_tabs(json_tabs)
		elif is_unexpected_output(path):
			aggregated_output.add_test_failure(path)
	write_aggregated_output(aggregated_output, path_to_output)
