document.addEventListener('DOMContentLoaded', function() {
    document.getElementById('UpdateHeaderButton').addEventListener('click', function() {
        let input = document.querySelector('#UpdateHeaderInput');
        document.getElementById('CustomHeader').innerHTML = input.value;
        });
    })
