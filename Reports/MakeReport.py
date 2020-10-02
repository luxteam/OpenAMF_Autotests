from jinja2 import Environment, FileSystemLoader, select_autoescape
import sys
import json
import os

if not os.path.exists('report'):
    os.mkdir('report')

# Reading from all .json files with results from all GPUs (passed as arguments)
statistics = list()
for i in range(1, len(sys.argv)):
    results_json = sys.argv[i]
    with open(results_json, 'r') as results_file:
        statistics.append(json.load(results_file))

env = Environment(
    loader=FileSystemLoader('templates')
)

### Creating main page ###    
template = env.get_template('main_page.html')

# Forming information for 'Summary' section
gpu_stat = list()
for result in statistics:
    gpu_stat.append(
        {
        'GPU' : 'GPU_NAME-OS',
        'Configuration' : 'CONFIGURATION',
        'Total' : result['tests'],
        'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
        'Failed' : result['failures'],
        'Disabled' : result['disabled'],
        'Error' : result['errors'],
        'Time taken': result['time'],
        'Test suites': result['testsuites']
        }
    )
out_file = template.render(gpu_stat=gpu_stat)

with open('report/realMainPage.html', 'w') as fh:
    fh.write(out_file)


# Forming information for 'Details' sections
template = env.get_template('test_suite_details_page.html')
for stat in statistics:
    for result in stat['testsuites']:

        for test_case in result['testsuite']:
            if 'DISABLED_' in test_case['name']:
                test_case['Result'] = 'Skipped'
            elif not 'failures' in test_case:
                test_case['Result'] = 'Passed'
            else:
                test_case['Result'] = 'Failure<br>'
                for fail in test_case['failures']:
                    test_case['Result'] += fail['failure']

        test_suite = {
            'GPU' : 'GPU_NAME-OS',
            'Configuration' : 'CONFIGURATION',
            'Name' : result["name"],
            'Total' : result['tests'],
            'Passed' : result['tests'] - result['failures'] - result['disabled'] - result['errors'],
            'Failed' : result['failures'],
            'Disabled' : result['disabled'],
            'Error' : result['errors'],
            'Time taken': result['time'],
            'Time stamp': result['timestamp'].replace('T', ' ').replace('Z', ''),
            'Test cases': result['testsuite']
            }
        
        out_file = template.render(test_suite=test_suite)

        with open(os.path.join('report', 'details_' + result['name'] + "_" + "GPU-OS" + '.html'), 'w') as fh:
            fh.write(out_file)
