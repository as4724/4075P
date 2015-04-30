clear timelength pushStep push resp
clear closeprc k pushintvl rspf f s colors pushSteps
%for WC
WC = importdata('WC-5min.asc');
WC_prices = WC.data;
Close = WC_prices(:,4);

timelength = floor(length(Close));
pushSteps = [100 500 1000];
colors = {'r','b','g','m','k'};

for s = 1:3   %time lag = pushStep * 5 mins
    pushStep = pushSteps(s);
    push = zeros(1,timelength-2*pushStep);
    resp = zeros(1,timelength-2*pushStep);

    closeprc = Close'*100;
    start=0;  % start time  you can set as your goal 

    %Difference calculation, original vale of push and response
    for i=1+pushStep:timelength-pushStep
        push(1,i-pushStep) = closeprc(1,i+start)-closeprc(1,i+start-pushStep);
        resp(1,i-pushStep) = closeprc(1,i+start+pushStep)-closeprc(1,i+start);
    end

    %[pdfPush,xi] = ksdensity(push);

    %Conditional sum of response function 'rspf'
    pushintvl = [min(push):0.25:max(push)];
    rspf = zeros(1,length(pushintvl));

    for k = 1:length(pushintvl)-1
        m = 0;

        for i=1:length(push);
            if ((pushintvl(k)<push(i))&&(push(i)<=pushintvl(k+1)))
                m = m+1;
                rspf(k) = rspf(k)+resp(i);
            end
        end
        if m~=0
            rspf(k) = rspf(k)/m;
        end
    end
    m1 = mean(push.^2);
    m2 = mean(resp.^2);
    sigma1 = sqrt(m1);
    sigma2 = sqrt(m2);
    %chart ploting
    plot(pushintvl/sigma1/100,rspf/sigma2/100, 'Color',colors{s}); %standardize by volatility
    % title(['Push-Respones Test; ', num2str(pushStep*5),' minutes '])

    hold on
    %f=fit(pushintvl',rspf','poly1');
    %plot(f,pushintvl,rspf);
    % f = polyfit(pushintvl',rspf',3);
    % x1 = linspace(min(pushintvl),max(pushintvl));
    % y1 = polyval(f, x1);
    % plot(x1, y1, 'r');
    % hold off

end;
grid on
hold off
% axis('tight');
title('Push-Response Test')
xlabel('Push')
xlim([-0.5 0.5])
ylim([-0.5 0.5])
ylabel('Response')
legend([num2str((pushSteps*5)'),repmat(' mins',3,1)],...
    'Orientation','horizontal','Location','SouthOutside');
