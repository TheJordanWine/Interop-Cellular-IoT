const assert = require('assert');

describe('localhost:3000 home webpage', () => {
    it('Should have an indentifier wheter the IN-CSE Server is running', () => {
        browser.url('http://localhost:3000');
        browser.waitUntil(() => {
            return $('span.badge.badge-success').isExisting();
        }, 10000, 'expected to have successful status');
        var statusButton = $('span.badge.badge-success');
        assert.equal(statusButton.getText(), 'Running');
    });
});
